﻿#include "HackerShell.h"
#include "ConsoleWidget.h"
#include "HackSession.h"
#include "NetworkManager.h"
#include "Shell.h"

void UHackerShell::UnbindEvents()
{
	// unbind from text submissions to prevent an access violation later on when we return control
	// to the game's main shell.
	//
	// This is a fucking trash way of implementing IO for a terminal but it's the best way of
	// doing it without doing weird shit.
	this->GetConsole()->OnTextSubmitted.RemoveDynamic(this, &UHackerShell::HandleCommandLine);
}

bool UHackerShell::HandleCommand(FString InName, TArray<FString> InArguments)
{
	if (InName == "exit")
	{
		this->UnbindEvents();
		this->Complete();
		return true;
	}
	else if (InName == "devmap")
	{
		if (InArguments.Num())
		{
			FString port = InArguments[0];

			this->GetConsole()->WriteLine(FText::FromString("analyzing device listening on port " + port + "..."));

			UHackableinfo* hackableInfo = nullptr;

			for (UHackableinfo* h : this->CurrentHack->GetHackables())
			{
				if (FString::FromInt(h->GetPort()) == port)
				{
					hackableInfo = h;
					break;
				}
			}

			if (hackableInfo)
			{
				this->GetConsole()->WriteLine(FText::GetEmpty());
				this->GetConsole()->WriteLine(FText::FromString("service on port " + port + " is " + hackableInfo->GetHackableId() + "(" + hackableInfo->GetHackableName() + ")."));
				this->GetConsole()->WriteLine(FText::GetEmpty());
				if (hackableInfo->GetHasFirewall())
				{
					this->GetConsole()->WriteLine(FText::FromString("Connection refused."));
				}
				else
				{
					this->GetConsole()->WriteLine(FText::FromString("no firewall detected"));
					this->GetConsole()->WriteLine(FText::GetEmpty());
					this->GetConsole()->WriteLine(FText::FromString("DEVICE INFORMATION"));
					this->GetConsole()->WriteLine(FText::FromString("=================="));
					this->GetConsole()->WriteLine(FText::GetEmpty());
					this->GetConsole()->WriteLine(FText::FromString("name: " + hackableInfo->GetDeviceName()));
					this->GetConsole()->WriteLine(FText::FromString("local IP: " + hackableInfo->GetDeviceAddress()));
					this->GetConsole()->WriteLine(FText::GetEmpty());

					if (hackableInfo->GetHasTrace())
					{
						this->GetConsole()->WriteLine(FText::FromString("%2\\warn YOU'RE BEING TRACED! \\warn%1"));
						this->CurrentHack->BeginTrace(hackableInfo);
					}
					
				}
			}
			else
			{
				this->GetConsole()->WriteLine(FText::FromString("error: Connection timed out."));
			}
		}
		else
		{
			this->GetConsole()->WriteLine(FText::FromString("error: missing port argument"));
		}

		return true;
	}
	else if (InName == "analyze")
	{
		this->GetConsole()->WriteLine(FText::FromString("Analyzing network for hackables..."));
		this->GetConsole()->WriteLine(FText::GetEmpty());

		for (UHackableinfo* hackable : this->CurrentHack->GetHackables())
		{
			this->GetConsole()->WriteLine(FText::FromString(hackable->GetHackableId() + "  on port " + FString::FromInt(hackable->GetPort())));
		}

		return true;
	}

	return false;
}

void UHackerShell::HandleCommandLine(FString InCommandLine)
{
	TArray<FString> tokens;
	FString error;
	if (UShell::BreakCommandLine(InCommandLine, tokens, error))
	{
		if (tokens.Num())
		{
			FString name = tokens[0];
			tokens.RemoveAt(0);

			if (!this->HandleCommand(name, tokens))
			{
				this->GetConsole()->WriteLine(FText::FromString(this->GetCommandName() + ": " + name + ": unrecognized command"));
			}
		}
	}
	else
	{
		this->GetConsole()->WriteLine(FText::FromString(error));
	}

	if (!this->IsCompleted())
	{
		this->WritePrompt();
	}
}

void UHackerShell::WritePrompt()
{
	this->GetConsole()->Write(FText::FromString(this->GetCommandName() + ":" + this->HackedHost + " >> "));
}

void UHackerShell::NativeRun()
{
	// get an ip/host from arguments.
	if (this->GetArguments().Num())
	{
		this->HackedHost = this->GetArguments()[0];
	}
	else
	{
		this->GetConsole()->WriteLine(FText::FromString("%2error: missing hostname/ip%1"));
		this->Complete();
		return;
	}

	// Start a hack session on that IP address we found above.
	UHackSession* hackSession = this->GetNetworkManager()->BeginHack(this->GetUserContext(), this->HackedHost);
	
	// if we get nullptr then the host was unreachable.
	if (!hackSession)
	{
		this->GetConsole()->WriteLine(FText::FromString("%2error: host unreachable%1"));
		this->Complete();
		return;
	}

	// keep the hacking session on record because duh
	this->CurrentHack = hackSession;

	// Let's make sure the fucking console gives us the fucking text input we fucking need in this fucking program.
	this->GetConsole()->OnTextSubmitted.AddUniqueDynamic(this, &UHackerShell::UHackerShell::HandleCommandLine);

	// let's act just like a shell BECAUSE WE ARE A FUCKING SHELL
	this->WritePrompt();
}
