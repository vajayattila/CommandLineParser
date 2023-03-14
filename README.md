
# CommandLineParser    
CommandLineParser is a C++ class that allows users to easily handle command-line arguments. The class enables defining new options that the program can recognize and specifies whether the options are required or optional. Descriptions can also be assigned to the options to help users understand what they are for.

The class allows for checking mutually exclusive options and displays error messages.

The class allows users to query the status and value of the options.
## C++ Compiler compatibility
1.  GCC (GNU Compiler Collection) - Not tested
2.  Clang (C/C++/Objective-C Compiler) - Not tested
3.  Visual C++ (Microsoft) - **Tested**
4.  Intel C++ Compiler - Not tested
5.  LLVM (Low Level Virtual Machine) - Not tested
6.  MinGW (Minimalist GNU for Windows) - Not tested
7.  Cygwin (Unix-like environment and command-line interface for Microsoft Windows) - Not tested
8.  Code::Blocks (Integrated Development Environment) - Not tested
## Public functions
### `void AddOption(const std::string& option_name, const std::string& option_description, const std::vector<std::string>& aliases = std::vector<std::string>(), bool required=false, const std::vector<std::string>& mutually_exclusives =std::vector<std::string>())`  
  
The method used for adding options. With the help of the class, we can define new options that the program can recognize and specify whether the options are mandatory or optional. Descriptions can also be assigned to the options so that the user can understand what the options are for. The options can also be mutually exclusive, which can also be specified.  
  
### `void Parse(int argc, char* argv[])`  
  
The method used to process command-line arguments. The method uses the `argc` and `argv` parameters of the `main()` function to read in the command-line arguments. The method checks if the options were correctly specified and sets the values of the options to the provided values.  
  
### `std::string GetOptionValue(const std::string& option_name) const`  
  
Method used to query the value of an option. The name of the option must be passed as a parameter, and the method returns the value assigned by the user to that option.  
  
### `std::string GetOptionDescription(const std::string& option_name) const`  
  
Method used to query the description of an option. The option name must be passed as a parameter, and the method returns the description of the option.  
  
### `bool HasOption(const std::string& option_name) const`  
  
Method for checking if an option is used. The name of the option should be passed as a parameter and it returns whether the option is used or not.  
  
## Using the `CommandLineParser` class
The usage of the `CommandLineParser` class is as follows:
 1. Create a `CommandLineParser` object.
 2. Define one or more command line options using the `AddOption()` function.
 3. Call the `Parse()` function with the argc and argv parameters.
 4. Check if an option is set by using the `HasOption` function with the option name.
 5. Call the `GetOptionValue()` function with the option name if necessary.  
### Creating a `CommandLineParser` object  
We can create a `CommandLineParser` object without parameters, in which case the value of the `programname_` variable will be "[program]". If we provide a string as a parameter, then the value of the `programname_` variable will be set accordingly.  
```c++  
CommandLineParser parser;  
CommandLineParser parser("MyProgram");  
```  
### Add command line options  
Using the `AddOption()` function, we can define one or more command line options. We need to specify the name, description, and aliases of the option, as well as whether the option is mandatory and whether it belongs to a group of mutually exclusive options.  
```c++  
parser.AddOption("--foutput", "Output file name", {"-fo"});  
parser.AddOption("--verbose", "Enable verbose mode", {"-v"}, true);  
parser.AddOption("--finput", "Input file name", {"-fi"}, true, {"--foutput"});  
```  
### Processing command line arguments  
The `Parse()` function is used to process command line arguments. The function takes the `argc` and `argv` parameters as input.  
```c++  
int main(int argc, char* argv[]) {  
CommandLineParser parser("MyProgram");  
parser.AddOption("--foutput", "Output file name", {"-fo"});  
parser.AddOption("--verbose", "Enable verbose mode", {"-v"}, true);  
pars-er.AddOption("--finput", "Input file name", {"-fi"}, true, {"--foutput"});  
  
parser.Parse(argc, argv);  
  
// további kód ...  
}  
```  
#### Check for mandatory options  
The `required_options_` vector defined in the `CommandLineParser` class contains the names of all the options that must be provided. After processing the options in the loop in the `Parse` function, we check in another loop whether all the required options have been provided. If any required option is missing, the application exits and writes an error message to the standard error stream.
To specify a `required` option, set the `required` parameter of the `AddOption` function to `true`. Example:  
```cpp  
parser.AddOption("-finput", "Input file", {"-fi"}, true);  
```  
This code means that the `--fi` or `--finput` switch is required to be provided, and the application will throw an error without their usage. 
```  
Error: --finput is a required option.  
```  
#### Handling of mutually exclusive parameters
If the application has options that cannot be used together, then these options must be specified in the `mutually_exclusive` parameter of the `AddOption` function. Example: 
```cpp  
parser.AddOption("--foo", "Foo option", {"-f"});  
parser.AddOption("--bar", "Bar option", {"-b"});  
parser.AddOption("--baz", "Baz option", {"-z"}, false, {"--foo", "--bar"});  
```  
In this case, the `-z` or `--baz` and `-b` or `--bar` options exclude each other. If one is used, the other cannot be used. If we specify multiple mutually exclusive options, the application will throw an error and exit. The error message is as follows:
```  
Error: Options "--baz" and "--bar" are mutually exclusive.  
```  
### Querying the setting and value of the options  
The `HasOption` function of the `CommandLineParser` class allows the application to check whether an option is set. The `GetOptionValue` function can be used to retrieve the value of an option, with the parameter being the name of the option. If the option was not set, the function returns an empty string.
```c++  
CommandLineParser parser;  
parser.AddOption("--finput", "Input file", {"-fi"}, true);  
parser.AddOption("--foutput", "Output file", {"-fo"});  
parser.Parse(argc, argv);  
  
if (parser.HasOption("--finput")) {  
std::string input_file = parser.GetOptionValue("--finput");  
std::cout << "Input file: " << input_file << std::endl;  
} else {  
std::cout << "Input file is not set" << std::endl;  
}  
  
if (parser.HasOption("--foutput")) {  
std::string output_file = parser.GetOptionValue("--foutput");  
std::cout << "Output file: " << output_file << std::endl;  
} else {  
std::cout << "Output file is not set" << std::endl;  
}  
```  
In the previous example, we can see that first we check if the `--finput` option is set using the `HasOption` function. If it is, then we get its value using the `GetOptionValue` function and print the result to the console. We do the same thing with the `--foutput` option. If neither option is set, then we display a message indicating that the option is not set.  
## Complete sample code
```c++  
#include "CommandLineParser.hpp"  
#include <iostream>  
  
int main(int argc, char* argv[]) {  
CommandLineParser parser("my_program");  
parser.AddOption("--finput", "Input file", {"-fi"}, true);  
parser.AddOption("--foutput", "Output file", {"-fo"}, false);  
parser.AddOption("--verbose", "Verbose output", {"-v"}, false);  
parser.AddOption("--debug", "Debug mode", {"-d"}, false, {"--verbose"});  
  
parser.Parse(argc, argv);  
  
if (parser.HasOption("--verbose")) {  
std::cout << "Verbose output enabled" << std::endl;  
}  
  
if (parser.HasOption("--debug")) {  
std::cout << "Debug mode enabled" << std::endl;  
}  
  
std::string input_file = parser.GetOptionValue("--finput");  
std::cout << "Input file: " << input_file << std::endl;  
  
if (parser.HasOption("--foutput")) {  
std::string output_file = parser.GetOptionValue("--foutput");  
std::cout << "Output file: " << output_file << std::endl;  
}  
  
return 0;  
}  
```  

This code initializes a `CommandLineParser` object, adds some options, where `--finput` is mandatory and `--foutput` is optional. The `--verbose` and `--debug` switches are also added and the two cannot be used together.

Then we call the `Parse()` function to parse the command line arguments. We check whether the switches are set with the `HasOption()` function and retrieve the option value with the `GetOptionValue()` function.

The above example demonstrates the use of `CommandLineParser` with the help of the most important functions. The application cannot be run without the `--finput` switch, while the `--foutput`, `--verbose`, `--debug` switches are optional. The `--debug` switch can only be set if `--verbose` is not set.
## License
The `CommandLineParser` code is licensed under the `MIT` license, which is a permissive open-source license that allows for commercial use, modification, distribution, and private use. The license only requires that the original copyright and license notice be included in the distribution. It provides users with maximum freedom to use and modify the code as they see fit.
