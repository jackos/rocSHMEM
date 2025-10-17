#!/bin/bash

# set -e

# Install Clang 20 for building rocSHMEM bitcode libs
echo "Installing Clang 20..."
wget https://apt.llvm.org/llvm.sh && chmod +x llvm.sh && sudo ./llvm.sh 20
rm llvm.sh

export ROCM_DIR=/opt/rocm

# Get the directory where this script is located
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Pin to specific version of UCX and OMPI
export UCX_INSTALL_DIR=$SCRIPT_DIR/ucx_install
export UCX_REPO=https://github.com/ROCm/ucx.git
export UCX_COMMIT_HASH=18770fdc1c3b5de202d14a088a14b734d2c4bbf3

export OMPI_INSTALL_DIR=$SCRIPT_DIR/mpi_install
export OMPI_REPO=https://github.com/ROCm/ompi.git
export OMPI_COMMIT_HASH=697a596dde68815fe50db3c2a75a42ddb41b5ef4

export ROCSHMEM_INSTALL_DIR=$SCRIPT_DIR/rocshmem_install
export ROCSHMEM_REPO=https://github.com/modular/rocSHMEM.git
export ROCSHMEM_BRANCH=bitcode_and_shared_lib

# # Step 1: Build UCX with ROCm support
cd $SCRIPT_DIR
if [ -d "ucx" ]; then
    # Ask user if they want to upload artifacts
    read -p "ucx already cloned, do you want to rebuild?" -n 1 -r
    echo ""

    if [[ $REPLY =~ ^[Yy]$ ]]; then
        rm -rf ucx
        git clone $UCX_REPO
        cd ucx
        git checkout $UCX_COMMIT_HASH
        ./autogen.sh
        ./contrib/configure-release \
            --prefix=$UCX_INSTALL_DIR \
            --with-rocm=$ROCM_DIR     \
            --enable-mt                \
            --without-go               \
            --without-java             \
            --without-knem
        make -j
        make install
    fi
fi

# Step 2: Install OpenMPI with UCX support
cd $SCRIPT_DIR
if [ -d "ompi" ]; then
    # Ask user if they want to upload artifacts
    read -p "ompi already cloned, do you want to rebuild?" -n 1 -r
    echo ""

    if [[ $REPLY =~ ^[Yy]$ ]]; then
        echo "Cloning OpenMPI..."
        git clone --recursive $OMPI_REPO

        cd ompi
        git checkout $OMPI_COMMIT_HASH
        git submodule update --init --recursive

        curl -LsSf https://astral.sh/uv/install.sh | sh
        $HOME/.local/bin/uv venv -c
        source .venv/bin/activate
        $HOME/.local/bin/uv pip install -r docs/requirements.txt
        ./autogen.pl
        ./configure --prefix=$OMPI_INSTALL_DIR  \
            --with-rocm=$ROCM_DIR       \
            --with-ucx=$UCX_INSTALL_DIR \
            --disable-oshmem             \
            --with-prrte=internal        \
            --with-hwloc=internal        \
            --with-libevent=internal     \
            --disable-mpi-fortran        \
            --without-ofi
        make -j
        make install
    fi
fi

# Step 3: Install rocshmem
cd $SCRIPT_DIR
mkdir -p build

export MPI_ROOT=$OMPI_INSTALL_DIR
export UCX_ROOT=$UCX_INSTALL_DIR
export CMAKE_PREFIX_PATH=$ROCSHMEM_INSTALL_DIR

cmake \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=$ROCSHMEM_INSTALL_DIR \
    -DCMAKE_VERBOSE_MAKEFILE=OFF \
    -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
    -DDEBUG=OFF \
    -DPROFILE=OFF \
    -DUSE_IPC=ON \
    -DUSE_THREADS=ON \
    -DUSE_WF_COAL=OFF \
    -DUSE_HDP_FLUSH=OFF \
    -DUSE_HDP_FLUSH_HOST_SIDE=OFF \
    -DUSE_RO=ON \
    -DBUILD_BITCODE_LIBRARY=ON \
    -DBUILD_SHARED_LIBS=ON \
    -G Ninja -S . \
    -B build

cmake --build build --target $ROCSHMEM_INSTALL_DIR


# # Step 4: Upload artifacts
# read -p "Do you want to upload build artifacts to S3? (y/n): " -n 1 -r
# echo ""

# if [[ $REPLY =~ ^[Yy]$ ]]; then
#   echo -e "Preparing to upload artifacts to S3..."
#   tar -czf $NVSHMEM_BUILD_TAR -C $INSTALL_DIR .
#   echo -e "Archived to $NVSHMEM_BUILD_TAR"

#   # Check if MODULAR_PATH is set, if not prompt for it
#   if [[ -z "$MODULAR_PATH" ]]; then
#     echo -e "MODULAR_PATH environment variable is not set."
#     read -p "Please enter the path to the Modular repository: " MODULAR_PATH

#     # Validate the entered path
#     if [[ ! -d "$MODULAR_PATH" ]]; then
#       echo -e "Error: The path '$MODULAR_PATH' does not exist or is not a directory."
#       exit 1
#     fi

#     # Check if the upload script exists at the expected location
#     if [[ ! -f "$MODULAR_PATH/utils/upload-public-bazel-artifact.sh" ]]; then
#       echo -e "Error: upload-public-bazel-artifact.sh not found at '$MODULAR_PATH/utils/'"
#       exit 1
#     fi
#   fi

#   # Call the upload script
#   ${MODULAR_PATH}/utils/upload-public-bazel-artifact.sh nvshmem "$VERSION" ./nvshmem.tar.gz

#   echo -e "Use above url and hash to update nvshmem bazel in: modular/open-source/max/bazel/common.MODULE.bazel"
# else
#   echo -e "Skipping artifact upload."
#   echo -e "Build artifacts are available at: ${INSTALL_DIR}"
# fi
