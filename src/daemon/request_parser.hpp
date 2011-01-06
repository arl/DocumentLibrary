#ifndef DOCLIB_DAEMON_REQUEST_PARSER_HPP
#define DOCLIB_DAEMON_REQUEST_PARSER_HPP

#include <boost/logic/tribool.hpp>
#include <boost/tuple/tuple.hpp>
#include <string>

namespace doclib
{

	namespace daemon
	{

		struct request;

		/**
		* @brief Parser for incoming request
		*/
		class request_parser
		{
			public:

				/**
				* @brief Construct ready to parse the request method.
				*/
				request_parser();

				/**
				* @brief Reset to initial parser state.
				*/
				void reset();

				/**
				* @brief parse some data
				*
				* @param req the parsed request
				* @param begin InputIterator first
				* @param end InputIterator end
				*
				* @return  The tribool return value is true when a complete request
				*	has been parsed, false if the data is invalid, indeterminate when more
				*	data is required.
				*/
					template <typename InputIterator>
				boost::tuple<boost::tribool, InputIterator> parse(request& req,
						InputIterator begin, InputIterator end)
				{
					while (begin != end)
					{
						boost::tribool result = consume(req, *begin++);
						if (result || !result)
							return boost::make_tuple(result, begin);
					}
					boost::tribool result = boost::indeterminate;
					return boost::make_tuple(result, begin);
				}

			private:

				/**
				* @brief Convert an action string to an enum value
				*
				* @param str received action string
				*
				* @return action enum value (or -1 for error).
				*/
				int convert_action_string(const std::string & str);

				/**
				* @brief Handle the next character of input.
				*
				* @param req currently forged request
				* @param input input character to be treated
				*
				* @return 
				*/
				boost::tribool consume(request& req, char input);

				/**
				* @brief Check if a byte is a printable character.
				*
				* @param c input character
				*/
				static bool is_char(int c);

				/**
				* @brief Check if a byte is a control character.
				*
				* @param c input character
				*/
				static bool is_ctl(int c);

				/**
				* @brief Check if a byte is defined as a special character.
				*
				* @param c input character
				*/
				static bool is_tspecial(int c);

				/**
				* @brief Check if a byte is a digit.
				*
				* @param c input character
				*/
				static bool is_digit(int c);

				/**
				* @brief Check if a byte is a separator.
				*
				* @param c input character
				*/
				static bool is_sep(int c);

				/**
				* @brief The current state of the parser.
				*/
				enum state
				{
					action_start,
					action,
					action_end,
					count_args_start,
					count_args,
					arg_size_start,
					arg_size,
					arg_data,
					arg_data_end,
					packet_end
                } state_;
		};

	}

}

#endif // !DOCLIB_DAEMON_REQUEST_PARSER_HPP
