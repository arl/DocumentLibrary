#ifndef DOCLIB_CORE_DOCLIB_EXCEPTION_HPP
#define DOCLIB_CORE_DOCLIB_EXCEPTION_HPP

#include "core_includes.hpp"

namespace doclib
{

	namespace core
	{

		using namespace std;


		class doclib_exception : public std::exception
		{
			public:

				doclib_exception(string msg)
				{
					_msg = "exception thrown : ";
					_msg.append(msg);
					log_exception();
				}

				virtual ~doclib_exception() throw()
				{
				}

				virtual const char* what() const throw()
				{
					return _msg.c_str();
				}

			private:

				void log_exception()
				{
					LERR_ << "exception thrown : " << _msg;
				}

				string _msg;

		};

	}

}

#endif // !DOCLIB_CORE_DOCLIB_EXCEPTION_HPP
