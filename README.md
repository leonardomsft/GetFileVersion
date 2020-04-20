# GetFileVersion

GetFileVersion helps diagnose DLL version issues by displaying complete versioning information for a binary file.

Versioning information can be stored in binary files in two different ways: 

1. Fixed portion, stores file version in numerical format 
2. Variable portion, stores file version in text format

This can lead to confusion, since the fixed portion may be different than the variable portion.

This happens because when the binary file being querried has a MUI file associated with it, the variable portion comes from the MUI file, and not the binary file itself.

For example, right-clicking this DLL -> properties shows that the version is 10.0.18362.752

![Alt text](screenshot2.png?raw=true "Image2")

However, Powershell shows that the version is 10.0.18362.1

![Alt text](screenshot3.png?raw=true "Image3")


GetFileVersion displays both the fixed and the variable portions:

![Alt text](screenshot1.png?raw=true "Image1")







#References

GetFileVersionInfoA function
https://docs.microsoft.com/en-us/windows/win32/api/winver/nf-winver-getfileversioninfoa
