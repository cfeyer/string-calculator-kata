#ifndef ADD_OBSERVER_INTERFACE_H
#define ADD_OBSERVER_INTERFACE_H

#include <string>

class Add_Observer_Interface
{
	public:
		virtual ~Add_Observer_Interface() {}

		virtual void add_occurred( const std::string & expression, int result ) = 0;
};

#endif /*ADD_OBSERVER_INTERFACE_H*/

