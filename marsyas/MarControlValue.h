/*
** Copyright (C) 1998-2006 George Tzanetakis <gtzan@cs.uvic.ca>
**  
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
** 
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
** 
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software 
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

/** 
	\class MarControlValue
	\brief Generic class for Marsyas control values
	Created by lfpt@inescporto.pt and lmartins@inescporto.pt
*/

#ifndef __MARCONTROLVALUE__
#define __MARCONTROLVALUE__

#include <string>
#include "common.h"
#include "realvec.h"

namespace Marsyas
{

class MarControlValue
{
protected:
	std::string type_;

protected:
	MarControlValue() {} // can't be directly created (use MarControl or MarControlValueT)

public:
	virtual ~MarControlValue() {}

	virtual MarControlValue* clone() = 0;
	virtual MarControlValue* create() = 0;

	virtual std::string getTypeID() = 0;
	// workaround method to avoid circular dependencies
	std::string getRegisteredType();

	virtual std::string	getType() const ;

	//virtual MarControlValue*(*)(std::istream&) getStreamCreationFunc() = 0;

	// workaround - virtual member functions to overload friend operators
	virtual void createFromStream(std::istream&) = 0;
	// for:	friend std::ostream& operator<<(std::ostream&, const MarControl& ctrl);
	virtual std::ostream& serialize(std::ostream& os) = 0;
	// for:	friend bool operator!=(MarControlValue& v1, MarControlValue& v2)
	virtual bool isNotEqual(MarControlValue *v) = 0;

	virtual MarControlValue* sum(MarControlValue *v) = 0;
	virtual MarControlValue* subtract(MarControlValue *v) = 0;
	virtual MarControlValue* multiply(MarControlValue *v) = 0;
	virtual MarControlValue* divide(MarControlValue *v) = 0;
};

//////////////////////////////////////////////////////////////////////////

template<class T>
class MarControlValueT : public MarControlValue
{
	friend class MarControl;

protected:
	T value_;

public:
	static T invalidValue;

public:
	MarControlValueT();
	MarControlValueT(T value);
  MarControlValueT(const MarControlValueT& a);

	virtual ~MarControlValueT() {}

	MarControlValueT& operator=(const MarControlValueT& a);

	virtual MarControlValue* clone();
	virtual MarControlValue* create();
	
	virtual std::string getTypeID();

	//setters
	void set(MarControlValue *val);
	void set(T &re);

	//getters
	const T& get() const;
  
	virtual void createFromStream(std::istream&);
	virtual std::ostream& serialize(std::ostream& os);
	virtual bool isNotEqual(MarControlValue *v);
	virtual MarControlValue* sum(MarControlValue *v);
	virtual MarControlValue* subtract(MarControlValue *v);
	virtual MarControlValue* multiply(MarControlValue *v);
	virtual MarControlValue* divide(MarControlValue *v);
	bool isInvalid() { return &value_ == &invalidValue; }
};

template<>
class MarControlValueT<realvec> : public MarControlValue
{
	friend class MarControl;

protected:
	realvec value_;

public:
	static realvec invalidValue;

public:
	MarControlValueT(realvec value);
	MarControlValueT(const MarControlValueT& a);

	virtual ~MarControlValueT() {}

	MarControlValueT& operator=(const MarControlValueT& a);

	virtual MarControlValue* clone();
	virtual MarControlValue* create();

	virtual std::string getTypeID();

	//setters
	inline void set(MarControlValue *val);
	inline void set(realvec &re);

	//getters
	const realvec& get() const;
	realvec& getRef();

	virtual void createFromStream(std::istream&);
	virtual std::ostream& serialize(std::ostream& os);
	virtual bool isNotEqual(MarControlValue *v);
	virtual MarControlValue* sum(MarControlValue *v);
	virtual MarControlValue* subtract(MarControlValue *v);
	virtual MarControlValue* multiply(MarControlValue *v);
	virtual MarControlValue* divide(MarControlValue *v);
	bool isInvalid() { return &value_ == &invalidValue; }
};

// To avoid the compiler complaints
template<>
class MarControlValueT<bool> : public MarControlValue
{

protected:
	bool value_;

public:
	static bool invalidValue;

public:
	MarControlValueT(bool value);
	MarControlValueT(const MarControlValueT& a);

	virtual ~MarControlValueT() {}

	MarControlValueT& operator=(const MarControlValueT& a);

	virtual std::string getTypeID();

	virtual MarControlValue* clone();
	virtual MarControlValue* create();

	//setters
	inline void set(MarControlValue *val);
	inline void set(bool &re);

	//getters
	const bool& get() const;

	virtual void createFromStream(std::istream&);
	virtual std::ostream& serialize(std::ostream& os);
	virtual bool isNotEqual(MarControlValue *v);
	virtual MarControlValue* sum(MarControlValue *v);
	virtual MarControlValue* subtract(MarControlValue *v);
	virtual MarControlValue* multiply(MarControlValue *v);
	virtual MarControlValue* divide(MarControlValue *v);
	bool isInvalid() { return &value_ == &invalidValue; }
};

// To avoid the compiler complaints
inline std::string operator-(std::string& , std::string&) { return ""; }
inline std::string operator*(std::string& , std::string&) { return ""; }
inline std::string operator/(std::string& , std::string&) { return ""; }

/************************************************************************/
/* MarControlValueT template implementation                             */
/************************************************************************/
template<class T>
T MarControlValueT<T>::invalidValue;

template<class T>
MarControlValueT<T>::MarControlValueT()
{
	value_ = T();

	// simple tests are previously done for basic types for efficiency purposes
	if (typeid(T) == typeid(mrs_real))
		type_ = "mrs_real";
	else if (typeid(T) == typeid(mrs_natural))
		type_ = "mrs_natural";
	else if (typeid(T) == typeid(std::string))
		type_ = "mrs_string";
	else if (typeid(T) == typeid(realvec))
		type_ = "mrs_realvec";
	else if (typeid(T) == typeid(bool))
		type_ = "mrs_bool";
	else
	{
		type_ = this->getRegisteredType();		
	}
}

template<class T>
MarControlValueT<T>::MarControlValueT(T value)
{
	value_ = value;

	// simple tests are used for basic types for efficiency purposes
	if (typeid(T) == typeid(mrs_real))
		type_ = "mrs_real";
	else if (typeid(T) == typeid(mrs_natural))
		type_ = "mrs_natural";
	else if (typeid(T) == typeid(std::string))
		type_ = "mrs_string";
	else if (typeid(T) == typeid(realvec))
		type_ = "mrs_realvec";
	else if (typeid(T) == typeid(bool))
		type_ = "mrs_bool";
	else
	{
		type_ = this->getRegisteredType();		
	}
}

template<class T>
MarControlValueT<T>::MarControlValueT(const MarControlValueT& a)
{
	value_ = a.value_;
	type_ = a.type_;
}

template<class T>
MarControlValueT<T>& 
MarControlValueT<T>::operator=(const MarControlValueT& a)
{
	if (this != &a)
	{
		value_ = a.value_;
		type_ = a.type_;
	}
	return *this;
}

template<class T>
MarControlValue*
MarControlValueT<T>::clone()
{
	return new MarControlValueT<T>(*this);
}

template<class T>
MarControlValue*
MarControlValueT<T>::create()
{
	return new MarControlValueT<T>(T());
}

template<class T>
std::string
MarControlValueT<T>::getTypeID()
{
	return typeid(T).name();
}

inline
std::string
MarControlValueT<realvec>::getTypeID()
{
	return typeid(realvec).name();
}

inline
std::string
MarControlValueT<bool>::getTypeID()
{
	return typeid(bool).name();
}

template<class T>
void
MarControlValueT<T>::set(T &val)
{
	value_ = val;
}

inline
void
MarControlValueT<realvec>::set(realvec &val)
{
	if(value_.getSize() != val.getSize()) 
	{
		value_.create(val.getSize());
	}
	value_ = val;
}

inline
void
MarControlValueT<bool>::set(bool &val)
{
	value_ = val;
}

template<class T>
const T&
MarControlValueT<T>::get() const
{
	return value_;
}

template<class T>
void
MarControlValueT<T>::createFromStream(std::istream& in)
{
	in >> value_;
}

template<class T>
bool
MarControlValueT<T>::isNotEqual(MarControlValue *v)
{
	if(this != v)//if referring to different objects, check if their contents is different...
	{
		if (type_ != v->getType())
		{
			std::ostringstream sstr;
			sstr << "[MarControlValueT::isNotEqual] Trying to compare different types of MarControlValue. "
				<< "(" << this->getType() << " with " << v->getType() << ")";
			MRSWARN(sstr.str());
			return false;
		}

		return value_ != dynamic_cast<MarControlValueT<T>*>(v)->get();
	}
	else //if v1 and v2 refer to the same object, they must be equal (=> return false)
		return false;
}

template<class T>
MarControlValue*
MarControlValueT<T>::sum(MarControlValue *v)
{
	MarControlValueT<T> *ptr = dynamic_cast<MarControlValueT<T>*>(v);
	if(!ptr)
	{
		std::ostringstream sstr;
		sstr << "[MarControlValueT::sum] Trying to sum different types of MarControlValue. "
			<< "(" << this->getType() << " with " << v->getType() << ")";
		MRSWARN(sstr.str());
		return false;
	}

	return new MarControlValueT<T>(value_+ptr->value_);
}

template<class T>
MarControlValue*
MarControlValueT<T>::subtract(MarControlValue *v)
{
	MarControlValueT<T> *ptr = dynamic_cast<MarControlValueT<T>*>(v);
	if(!ptr)
	{
		std::ostringstream sstr;
		sstr << "[MarControlValueT::subtract] Trying to subtract different types of MarControlValue. "
			<< "(" << this->getType() << " with " << v->getType() << ")";
		MRSWARN(sstr.str());
		return false;
	}

	return new MarControlValueT<T>(value_+ptr->value_);
}

template<class T>
MarControlValue*
MarControlValueT<T>::multiply(MarControlValue *v)
{
	MarControlValueT<T> *ptr = dynamic_cast<MarControlValueT<T>*>(v);
	if(!ptr)
	{
		std::ostringstream sstr;
		sstr << "[MarControlValueT::multiply] Trying to multiply different types of MarControlValue. "
			<< "(" << this->getType() << " with " << v->getType() << ")";
		MRSWARN(sstr.str());
		return false;
	}

	return new MarControlValueT<T>(value_*ptr->value_);
}

template<class T>
MarControlValue*
MarControlValueT<T>::divide(MarControlValue *v)
{
	MarControlValueT<T> *ptr = dynamic_cast<MarControlValueT<T>*>(v);
	if(!ptr)
	{
		std::ostringstream sstr;
		sstr << "[MarControlValueT::divide] Trying to divide different types of MarControlValue. "
			<< "(" << this->getType() << " with " << v->getType() << ")";
		MRSWARN(sstr.str());
		return false;
	}

	return new MarControlValueT<T>(value_/ptr->value_);
}

template<class T>
std::ostream&
MarControlValueT<T>::serialize(std::ostream& os)
{
	os << value_;
	return os;
}

}//namespace Marsyas

#endif
