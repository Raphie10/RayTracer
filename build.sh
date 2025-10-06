$!
mkdir -p build
cd build
cmake  -DENABLE_O3=ON ..
cmake --build .
if [ $? -ne 0 ]; then
    echo "Build failed !"
    exit 1
fi
echo "Build complete !"
echo "You can create your own scene configuration here :"
echo "https://yanisprevost.github.io/RayTracer/"

