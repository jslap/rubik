set -ex

mkdir -p build-cov
pushd build-cov
# find . -type f -name *.gcno | xargs rm
find . -type f -name *.gcda | xargs rm
cmake -DCOVERAGE:BOOL=ON ..
make -j4

# Run the test with coverage 
./Rubik-Test
# Create an empty coverage entry file (.gcda) for each file compiled files (.gcno)
# This way, if a cpp has no executed code, it will contribute as 0% covered, instead as ignored
# find . -type f -name "*.gcno" -exec sh -c 'touch -a "${1%.gcno}.gcda"' _ {} \;

lcov --base-directory ../../ --directory . --capture --output-file coverage-raw.info
lcov --remove coverage-raw.info '*tests/*' '/usr/*' '*third/*' '/Applications/Xcode*.app/*'  --output-file coverage-clean.info


rm -rf ./outHtml
mkdir -p outHtml
genhtml  --prefix "/Users/js/Documents/Dev/Rubik/Rubik-lib" --demangle-cpp --ignore-errors source coverage-clean.info --legend --output-directory=outHtml
popd