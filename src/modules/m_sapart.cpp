// Sapart and +g support module by C.J.Edwards

#include <stdio.h>
#include <string>
#include "users.h"
#include "channels.h"
#include "modules.h"

/* $ModDesc: Provides support for unreal-style SAPART command */

Server *Srv;
	 
void handle_sapart(char **parameters, int pcnt, userrec *user)
{
	userrec* dest = Srv->FindNick(std::string(parameters[0]));
	if (dest)
	{
		for (int x = 0; x < strlen(parameters[1]); x++)
		{
				if ((parameters[1][0] != '#') || (parameters[1][x] == ' ') || (parameters[1][x] == ','))
				{
					Srv->SendTo(NULL,user,"NOTICE "+std::string(user->nick)+" :*** Invalid characters in channel name");
					return;
				}
		}

		Srv->SendOpers(std::string(user->nick)+" used SAPART to make "+std::string(dest->nick)+" part "+parameters[1]);
		Srv->PartUserFromChannel(dest,std::string(parameters[1]),std::string(dest->nick));
	}
}


class ModuleSapart : public Module
{
 public:
	ModuleSapart()
	{
		Srv = new Server;
		Srv->AddCommand("SAPART",handle_sapart,'o',2);
	}
	
	virtual ~ModuleSapart()
	{
		delete Srv;
	}
	
	virtual Version GetVersion()
	{
		return Version(1,0,0,1);
	}
	
};

// stuff down here is the module-factory stuff. For basic modules you can ignore this.

class ModuleSapartFactory : public ModuleFactory
{
 public:
	ModuleSapartFactory()
	{
	}
	
	~ModuleSapartFactory()
	{
	}
	
	virtual Module * CreateModule()
	{
		return new ModuleSapart;
	}
	
};


extern "C" void * init_module( void )
{
	return new ModuleSapartFactory;
}

