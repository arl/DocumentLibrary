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

		/// Parser for incoming requests.
		class request_parser
		{
			public:
				/// Construct ready to parse the request method.
				request_parser();

				/// Reset to initial parser state.
				void reset();

				/// Parse some data. The tribool return value is true when a complete request
				/// has been parsed, false if the data is invalid, indeterminate when more
				/// data is required. The InputIterator return value indicates how much of the
				/// input has been consumed.
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

                /// Convert an action from a request packet to its enum value (or -1 for error).
				int convert_action_string(const std::string & str);

				/// Handle the next character of input.
				boost::tribool consume(request& req, char input);

				/// Check if a byte is a printable character.
				static bool is_char(int c);

				/// Check if a byte is a control character.
				static bool is_ctl(int c);

				/// Check if a byte is defined as a special character.
				static bool is_tspecial(int c);

				/// Check if a byte is a digit.
				static bool is_digit(int c);

				/// Check if a byte is a separator.
				static bool is_sep(int c);

				/// The current state of the parser.
				enum state
				{
					action_start,
					action,
					action_end,
					count_args_start,
					count_args,
					arg_size_start,
					arg_size,
					//arg_data_start,
					arg_data,
					arg_data_end,
					packet_end
                } state_;
		};

	}

}

#endif // !DOCLIB_DAEMON_REQUEST_PARSER_HPP
