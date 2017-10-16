#include <iostream>
#include <string>
using namespace std;

//--------------------------------------
class LongLongInt
{
public:
	LongLongInt(string);
	LongLongInt(const LongLongInt& C);
	~LongLongInt();
	void display();

	bool operator > (LongLongInt &c);
	bool operator < (LongLongInt &c);
	LongLongInt operator + (LongLongInt &c);
	LongLongInt operator - (LongLongInt &c);
	LongLongInt operator * (LongLongInt &c);
	LongLongInt operator / (LongLongInt &c);
	
private:
	int length;
	char *data;
};

LongLongInt::LongLongInt(string str)
{
	cout << sizeof(str) << endl;
	data = new char[sizeof(str)];
	length = strlen(data);
	for (int i = 0; i < length; ++i)
	{
		data[i] = str[i];
	}
}

LongLongInt::LongLongInt(const LongLongInt& C)
{
	this->length = C.length;
	data = new char[length];
	for (int i = 0; i < length; ++i)
	{
		data[i] = C.data[i];
	}
}

LongLongInt::~LongLongInt()
{
	delete [] data;
}

void LongLongInt::display()
{
	cout << data << endl;
}

//--------------------------------------
/*class sLongLongInt : public LongLongInt
{
public:
	sLongLongInt();
	~sLongLongInt();
	
};*/

//--------------------------------------
/*class Compare
{
public:
	Compare(LongLongInt a):c0(a){}
	~Compare(){}
	bool operator > (LongLongInt &c);
	bool operator < (LongLongInt &c);
private:
	LongLongInt c0;
};*/

bool LongLongInt::operator > (LongLongInt &c)
{
	cout << data << '\t' << c.data << endl;
	cout << strlen(data) << '\t' << strlen(c.data) << endl;
	if (data[0] != 45 && c.data[0] == 45)
	{
		return true;
	}
	else if (data[0] == 45 && c.data[0] != 45)
	{
		return false;
	}
	else if (data[0] != 45 && c.data[0] != 45)
	{
		if (length > c.length)
			return true;
		else if (length < c.length)
			return false;
		else
		{
			for (int i = 1; i < length; ++i)
			{
				if (data[i] > c.data[i])
					return true;
				else if (data[i] < c.data[i])
					return false;
			}
		}
	}
	else
	{
		if (length < c.length)
			return true;
		else if (length > c.length)
			return false;
		else
		{
			for (int i = 1; i < length; ++i)
			{
				if (data[i] > c.data[i])
					return false;
				else if (data[i] < c.data[i])
					return true;
			}

		}
	}
	return false;
}

bool LongLongInt::operator < (LongLongInt &c)
{
	if (data[0] != 45 && c.data[0] == 45)
	{
		return false;
	}
	else if (data[0] == 45 && c.data[0] != 45)
	{
		return true;
	}
	else if (data[0] != 45 && c.data[0] != 45)
	{
		if (length > c.length)
			return false;
		else if (length < c.length)
			return true;
		else
		{
			for (int i = 1; i < length; ++i)
			{
				if (data[i] > c.data[i])
					return false;
				else if (data[i] < c.data[i])
					return true;
			}
		}
	}
	else
	{
		if (length < c.length)
			return false;
		else if (length > c.length)
			return true;
		else
		{
			for (int i = 1; i < length; ++i)
			{
				if (data[i] > c.data[i])
					return true;
				else if (data[i] < c.data[i])
					return false;
			}

		}
	}
	return false;	
}


//--------------------------------------
/*class Plus_minus
{
public:
	Plus_minus(LongLongInt a):c0(a){}
	~Plus_minus(){}
	string operator + (LongLongInt &c);
	string operator - (LongLongInt &c);
private:
	LongLongInt c0;
};*/

LongLongInt LongLongInt::operator + (LongLongInt &c)
{
	int r = 0;
	int l = (length > c.length) ? (length+1) : (c.length+1);
	cout << length << '\t' << c.length << endl;
	cout << "l=" << l << endl;
	char *rst;
	rst = new char[l];
	if (data[0] != 45 && c.data[0] == 45)
	{
		
	}
	else if (data[0] == 45 && c.data[0] != 45)
	{
		
	}
	else if (data[0] != 45 && c.data[0] != 45)
	{
		rst[0] = '+';
		for (int i = 0; i < l; ++i)
		{
			cout << "i=" << i << endl;
			if (length-i > 0 && c.length-i > 0)
			{
				int t = data[length-i] + c.data[c.length-i] + r;
				rst[l-i] = t%10;
				cout << "rst[l-i]=" << rst[l-i] << endl;
				r = t/10;
			}
			else if (c.length-i <= 0)
			{
				int t = data[length-i] + r;
				rst[l-i] = t%10;
				cout << "rst[l-i]=" << rst[l-i] << endl;
				r = t/10;
			}
			else
			{
				int t = c.data[c.length-i] + r;
				rst[l-i] = t%10;
				cout << "rst[l-i]=" << rst[l-i] << endl;
				r = t/10;	
			}
		}
	}
	else
	{
		rst[0] = '-';
		for (int i = 0; i < l; ++i)
		{
			if (length-i > 0 && c.length-i > 0)
			{
				int t = data[length-i] + c.data[c.length-i] + r;
				rst[l-i] = t%10;
				r = t/10;
			}
			else if (length-i > 0)
			{
				int t = data[length-i] + r;
				rst[l-i] = t%10;
				r = t/10;
			}
			else
			{
				int t = c.data[c.length-i] + r;
				rst[l-i] = t%10;
				r = t/10;	
			}
		}
	}

	string ans = rst;
	delete [] rst;
	return ans;
}

LongLongInt LongLongInt::operator - (LongLongInt &c)
{}

//--------------------------------------
/*class Mutiply_divide
{
public:
	Mutiply_divide(LongLongInt a):c0(a){}
	~Mutiply_divide(){}
	string operator * (LongLongInt &c);
	string operator / (LongLongInt &c);
private:
	LongLongInt c0;
};*/

LongLongInt LongLongInt::operator * (LongLongInt &c)
{}

LongLongInt LongLongInt::operator / (LongLongInt &c)
{}

