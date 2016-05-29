#include"valid.h"


bool Valid::isPasswordValid(string pass)
{
	bool validpass = false;
	bool lowerCheck = false;
	bool upperCheck = false;
	bool spaceCheck = false;
	bool digitCheck = false;
	if (pass.size() < 4)
	{
		return false;
	}
	for (int i = 0; i < pass.size(); i++)
	{
		if (islower(pass[i]))
		{
			lowerCheck = true;
		}
		if (isupper(pass[i]))
		{
			upperCheck = true;
		}
		if (pass[i] == ' ')
		{
			spaceCheck = true;
		}
		if (isdigit(pass[i]))
		{
			digitCheck = true;
		}
	}
	if (lowerCheck == true && upperCheck == true && digitCheck == true && spaceCheck == false)
	{
		validpass = true;
	}
	else
	{
		validpass = false;
	}
	return validpass;
}

bool Valid::isUsernameValid(string user)
{
	bool userValid = false;
	bool firstLetter = false;
	bool spaceCheck = false;
	for (int i = 0; i < user.size(); i++)
	{
		if (user[i] == ' ')
		{
			spaceCheck = true;
		}
	}
	if (isalpha(user[0]))
	{
		firstLetter = true;
	}
	if (user.size() > 0 && spaceCheck == false && firstLetter == true)
	{
		userValid = true;
	}
	return userValid;
}
