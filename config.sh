# Define custom utilities
# Test for macOS with [ -n "$IS_OSX" ]

function pre_build {
    # Any stuff that you need to do before you start building the wheels
    # Runs in the root directory of this repository.

    # Run ctest before creating the build
    mkdir build && pushd build
    cmake .. -DBUILD_TESTS=ON -DCMAKE_BUILD_TYPE=Release
    cmake --build .
    ctest --output-on-failure
    popd && rm -rf build
}

function run_tests {
    # Runs tests on installed distribution from an empty directory

    # pytest adds every directory up-to and including python/ into sys.path,
    # meaning that "import ecl" will import python/ecl and not the installed one.
    # This doesn't work because the _ecl.so module only exists in site-packages,
    # so we copy directories required by the tests out into its own temporary
    # directory.
    mkdir -p {.git,python}
    ln -s {..,$PWD}/bin
    ln -s {..,$PWD}/lib
    ln -s {..,$PWD}/test-data
    cp -R {..,$PWD}/python/tests
    python -m pytest python/tests
}
