/**
 * @file CommandLineParser.hpp
 *
 * @brief The definition of the CommandLineParser class for handling command-line parameters.
 *
 * @author Attila Vajay
 * @email vajay.attila@gmail.com
 * @git https://github.com/vajayattila/CommandLineParser.git 
 * @date 2023.03.14.
 * @version 1.0
 * @license MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, see the file LICENSE.
 */

#ifndef COMMANDLINEPARSER_H_
#define COMMANDLINEPARSER_H_
#pragma once

#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

/**
 * @class CommandLineParser
 *
 * @brief A class for handles command line parameters
 */
class CommandLineParser {
public:
	CommandLineParser() { 
		programname_ = "[program]"; 
	};
	CommandLineParser(const std::string prgname) {
		programname_ = prgname;
	};

	/**
	 * @brief Define a command line option.
	 *
	 * @param option_name Option name.
	 * @param option_description Description of the option.
	 * @param aliases Aliases of the option.
	 * @param required The option must be used or not.
	 * @param mutually_exclusives List of options that the option cannot be used with.
	 */
	void AddOption(const std::string& option_name, const std::string& option_description,
		const std::vector<std::string>& aliases = std::vector<std::string>(), bool required=false,
		const std::vector<std::string>& mutually_exclusives =std::vector<std::string>() ) {
		options_[option_name].description = option_description;
		options_[option_name].aliases.push_back(option_name);

		for (const auto& alias : aliases) {
			options_[option_name].aliases.push_back(alias);
		}

		for (const auto& mutuallyexclusive : mutually_exclusives) {
			options_[option_name].mutually_exclusive = true;
			options_[option_name].mutually_exclusive_options.push_back(mutuallyexclusive);
		}

		if (required) {
			required_options_.push_back(option_name);
		}
	};

	/**
	 * @brief Parsing command line parameters.
	 *
	 * @param argc The argc parameter of the "main" function.
	 * @param argv The argv parameter of the "main" function.
	 */
	void Parse(int argc, char* argv[]) {
		for (int i = 1; i < argc; ++i) {
			std::string arg = argv[i];

			if (arg == "-h" || arg == "--help") {
				PrintUsage();
				exit(0);
			}

			bool option_found = false;
			for (auto& pair : options_) {
				for (const auto& alias : pair.second.aliases) {
					if (arg == alias) {
						option_found = true;
						if (pair.second.mutually_exclusive) {
							for (const auto& mutually_exclusive_option : pair.second.mutually_exclusive_options) {
								if (options_[mutually_exclusive_option].is_set) {
									std::cerr << "Error: Options \"" << mutually_exclusive_option << "\" and \"" << arg
										<< "\" are mutually exclusive." << std::endl;
									PrintUsage();
									exit(1);
								}
							}
						}
						pair.second.is_set = true;

						if (i + 1 < argc && argv[i + 1][0] != '-') {
							pair.second.value = argv[i + 1];
							++i;
						}
					}
				}
			}

			if (!option_found) {
				std::cerr << "Error: Invalid option \"" << arg << "\"." << std::endl;
				PrintUsage();
				exit(1);
			}
		}


		// Check for required options
		for (const auto& option : required_options_) {
			if (!HasOption(option)) {
				std::cerr << "Error: " << option << " is a required option." << std::endl;
				exit(1);
			}
		}
	};

	/**
	 * @brief The value assigned to the option.
	 *
	 * @param option_name Option name.
	 * @return The value
	 */
	std::string GetOptionValue(const std::string& option_name) const {
		return options_.at(option_name).value;
	};

	/**
	 * @brief Request a description of the option.
	 *
	 * @param option_name Option name.
	 * @return The description
	 */
	std::string GetOptionDescription(const std::string& option_name) const {
		return options_.at(option_name).description;
	};

	/**
	 * @brief An option is in use or not.
	 *
	 * @param option_name Option name.
	 * @return In use or not
	 */
	bool HasOption(const std::string& option_name) const {
		return (options_.find(option_name) != options_.end()) ? (options_.at(option_name).is_set) : (false);
	};

private:
	struct Option {
		std::string description;
		std::vector<std::string> aliases;
		bool is_set = false;
		std::string value;
		bool mutually_exclusive = false;
		std::vector<std::string> mutually_exclusive_options;
	};

	std::map<std::string, Option> options_;
	std::vector<std::string> required_options_;
	std::string programname_;

	/**
	 * @brief Show usage information.
	 */
	void PrintUsage() const {
		std::cout << "Usage: " << programname_ <<" [options]" << std::endl;
		std::cout << "Options:" << std::endl;

		for (const auto& pair : options_) {
			std::cout << "  ";
			for (const auto& alias : pair.second.aliases) {
				std::cout << alias << ", ";
			}
			std::cout << std::endl << "      " << pair.second.description << std::endl;
			if (pair.second.mutually_exclusive) {
				std::cout << "      Mutually exclusive with:";
				for (const auto& mutually_exclusive_option : pair.second.mutually_exclusive_options) {
					std::cout << " " << mutually_exclusive_option;
				}
				std::cout << std::endl;
			}
		}
	};
};

#endif