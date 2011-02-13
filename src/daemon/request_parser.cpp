/* Copyright (C) 
* 2010 - Aurelien Rainone
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
* 
*/

#include "request_parser.hpp"
#include "request.hpp"
#include <iostream>
#include <boost/lexical_cast.hpp>

namespace doclib
{

	namespace daemon
	{

		/************************************************************************/
		/*  REQUEST SYNTAX

			BW : browse_folder(string folder)


		*/
		/************************************************************************/


		request_parser::request_parser()
			: state_(action_start)
		{
		}

		void request_parser::reset()
		{
			state_ = action_start;
		}

        int request_parser::convert_action_string(const std::string & str)
        {
            if (str == "VI")
                return request::view_document;
            else if (str == "DW")
                return request::download_document;
			else if (str == "QY")
				return request::search_query;			
			else if (str == "BW")
				return request::browse_folder;
            else
                return -1;
        }


		boost::tribool request_parser::consume(request& req, char input)
		{
		    static std::string cur_action;			// action being consmumed
		    static std::string cur_digits;			// digits being consumed
		    static std::size_t num_args = 0;		// total number of arguments
		    static std::size_t cur_arg = 0;			// currently consumed argument
			static std::size_t cur_arg_size = 0;	// current argument size in bytes
			static std::size_t bytes_read = 0;		// current argument consumed bytes
/*

			// DBG CODE
			std::cout << "------------------------------------";
			
			switch(state_)
			{
				case action_start :
					std::cout << "action_start" << std::endl;
					break;
				case action :
					std::cout << "action" << std::endl;
					break;
				case action_end :
					std::cout << "action_end" << std::endl;
					break;
				case count_args_start :
					std::cout << "count_args_start" << std::endl;
					break;
				case count_args :
					std::cout << "count_args" << std::endl;
					break;
				case arg_size_start :
					std::cout << "arg_size_start" << std::endl;
					break;
				case arg_size :
					std::cout << "arg_size" << std::endl;
					break;
				case arg_data :
					std::cout << "arg_data" << std::endl;
					break;
				case arg_data_end :
					std::cout << "arg_data_end" << std::endl;
					break;
				case packet_end :
					std::cout << "packet_end" << std::endl;
					break;
				default:
					std::cout << "default" << std::endl;
					break;
			}

			// DBG CODE
			std::cout << "cur_action : " << cur_action << " : cur_digits : " << cur_digits << std::endl;
			std::cout << "num_args : " << num_args << " : cur_arg : " << cur_arg << std::endl;
			std::cout << "cur_arg_size : " << cur_arg_size << " : bytes_read : " << bytes_read << std::endl;
*/
			switch (state_)
			{
                case action_start:

                    cur_action.clear();
					if (!is_char(input) || is_ctl(input) || is_tspecial(input))
					{
						return false;
					}
					else
					{
                        state_ = action;
						cur_action.push_back(input);
						return boost::indeterminate;
					}

				case action:

					if (!is_char(input) || is_ctl(input) || is_tspecial(input))
					{
						return false;
					}
					else
					{
                        state_ = action_end;
						cur_action.push_back(input);
						return boost::indeterminate;
					}

				case action_end:

					if (!is_sep(input))
					{
						return false;
					}
					else
					{
						int action = convert_action_string(cur_action);
						if (action == -1)
						{
						    return false;
						}
                        else
                        {
                            state_ = count_args_start;
                            req.action = action;
							return boost::indeterminate;
                        }
					}

                case count_args_start:

                    cur_digits.clear();
					if (!is_digit(input))
					{
						return false;
					}
					else
					{
                        state_ = count_args;
						cur_digits.push_back(input);
						return boost::indeterminate;
					}

				case count_args:

					cur_arg = 0;
					if (is_digit(input))
					{
						cur_digits.push_back(input);
						return boost::indeterminate;
					}
					else if (is_sep(input))
					{
						// end of 'number of arguments' field
						num_args = boost::lexical_cast<std::size_t>(cur_digits);
						if (num_args == 0)
							state_ = packet_end;
						else
							state_ = arg_size_start;
						return boost::indeterminate;
					}
					else
					{
						return false;
					}

				case arg_size_start:

                    cur_digits.clear();
					if (!is_digit(input))
					{
						return false;
					}
					else
					{
                        state_ = arg_size;
						cur_digits.push_back(input);
						return boost::indeterminate;
					}

				case arg_size:

					if (is_digit(input))
					{
						cur_digits.push_back(input);
						return boost::indeterminate;
					}
					else if (is_sep(input))
					{
						// end of 'argument size' field
						cur_arg_size = boost::lexical_cast<std::size_t>(cur_digits);
						bytes_read = 0;

						if (cur_arg_size == 0)
							state_ = arg_data_end;
						else
							state_ = arg_data;

						// add an argument
						req.args.push_back(std::string());
						req.arg_sizes.push_back(cur_arg_size);
						return boost::indeterminate;
					}
					else
					{
						return false;
					}

				case arg_data:

					req.args[cur_arg].push_back(input);
					if (++bytes_read == cur_arg_size)
						state_ = arg_data_end;	

					return boost::indeterminate;

				case arg_data_end:

					if (!is_sep(input))
					{
						return false;
					}
					else
					{
						// end of description argument, start a new one?
						if (++cur_arg < num_args)
						{
							// there are more args left
							state_ = arg_size_start;
						}
						else
						{
							// this was the last arg
							state_ = packet_end;
						}
						return boost::indeterminate;
					}

				case packet_end:

					if (is_sep(input))
					{
						return true;
					}
					else
					{
						return false;
					}

				default:
					return false;
			}
		}

		bool request_parser::is_char(int c)
		{
			return c >= 0 && c <= 127;
		}

		bool request_parser::is_ctl(int c)
		{
			return (c >= 0 && c <= 31) || (c == 127);
		}

		bool request_parser::is_tspecial(int c)
		{
			switch (c)
			{
				case '(': case ')': case '<': case '>': case '@':
				case ',': case ';': case ':': case '\\': case '"':
				case '/': case '[': case ']': case '?': case '=':
				case '{': case '}': case ' ': case '\t':
					return true;
				default:
					return false;
			}
		}

		bool request_parser::is_digit(int c)
		{
			return c >= '0' && c <= '9';
		}

		bool request_parser::is_sep(int c)
		{
			return c == '|';
		}

	}

}
