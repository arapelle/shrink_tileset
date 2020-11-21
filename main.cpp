#include <Magick++.h> 
#include <iostream> 

using namespace Magick; 

int main(int argc, char** argv)
{ 
    InitializeMagick(*argv);
  
    // Construct the image object. Seperating image construction from the 
    // the read operation ensures that a failure to read the image file 
    // doesn't render the image object useless. 
    Image image;
    try 
    { 
        // Read a file into image object 
        image.read("emona.png");
        // Crop the image to specified size (width, height, xOffset, yOffset)
        image.crop(Geometry(100,100, 100, 100));
        // Write the image to a file 
        image.write("output.png");
    } 
    catch (Exception& error)
    { 
        std::cout << "Caught exception: " << error.what() << std::endl;
        return EXIT_SUCCESS;
    } 
    std::cout << "EXIT SUCCESS" << std::endl;
    return EXIT_SUCCESS;
}
