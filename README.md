# VTFParser
Work in progress simple and modern library for parsing the Valve Texture Format.  

This is mainly for use in my other projects like VisTrace, and will be acompanied by a similar BSP parser in the future.  

When complete it will provide a fully OOP method of reading VTF high res texture data, 
including helpers in the image class for reading pixel values, and automatic handling of mipmaps.  
I'll most likely not add support for writing, or many of the other features of VTFs, as VTFLib, while old and not very nice to use, can already do everything you'd need and is applicable for most projects.  
This however is designed to not require dynamically linking to a dll, and to provide a more streamlined way of getting just the data needed for texturing.
