#include <err.h>
#include <getopt.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

char* optarg;
int opterr = 1;
int optind = 1;
int optopt;

static size_t shortIndex = 1;

static int handleShortOption(int argc, char* const argv[], const char* optstring, bool colonMode)
{
	char option = argv[optind][shortIndex++];

	while (*optstring)
	{
		if (*optstring == option && option != ':')
		{
			if (optstring[1] == ':')
			{
				bool optional = optstring[2] == ':';
				if (argv[optind][shortIndex])
				{
					optarg = argv[optind] + shortIndex;
					optind++;
					shortIndex = 1;
				}
				else if (!optional && optind + 1 < argc)
				{
					optarg = argv[optind + 1];
					optind += 2;
					shortIndex = 1;
				}
				else if (!optional)
				{
					optopt = option;
					if (opterr && !colonMode)
					{
						warnx("option requires an argument -- '%c'", option);
					}
					option = !colonMode ? '?' : ':';
				}
			}
			return option;
		}
		optstring++;
	}
	optopt = option;
	if (opterr && !colonMode)
	{
		warnx("invalid option -- '%c'", option);
	}
	return '?';
}

static int handleLongOption(char* option, const struct option* longopts, int* longindex, bool colonMode)
{
	size_t optionLength = strcspn(option, "=");
	for (size_t i = 0; longopts[i].name; i++)
	{
		if (strncmp(option, longopts[i].name, optionLength) == 0 && longopts[i].name[optionLength] == '\0')
		{
			if (option[optionLength] == '\0' && longopts[i].has_arg == required_argument)
			{
				if (opterr && !colonMode)
				{
					warnx("option '--%s' requires an argument", option);
				}
				return !colonMode ? '?' : ':';
			}	
			else if (option[optionLength] != '\0' && longopts[i].has_arg == no_argument)
			{
				if (opterr && !colonMode)
				{
					warnx("option '--%s' does not allow an arguemnt", longopts[i].name);
				}
				return '?';
			}
			if (option[optionLength] != '\0')
			{
				optarg = option + optionLength + 1;
			}
			if (longindex)
			{
				*longindex = i;
			}
			if (longopts[i].flag)
			{
				*longopts[i].flag = longopts[i].val;
				return 0;
			}
			else
			{
				return longopts[i].val;
			}
		}
	}
	warnx("unrecognized option '--%s'", option);
	return '?';
}

int getopt_long(int argc, char* const argv[], const char* optstring, const struct option* longopts, int* longindex)
{
	optarg = NULL;
	optopt = 0;

	if (!argv[optind] || *argv[optind] != '-' || argv[optind][1] == '\0')
	{
		return -1;
	}
	if (argv[optind][1] == '-' && argv[optind][2] == '\0')
	{
		optind++;
		return -1;
	}
	bool colonMode = false;
	if (*optstring == ':')
	{
		colonMode = true;
		optstring++;
	}

	if (argv[optind][1] != '-')
	{
		int result = handleShortOption(argc, argv, optstring, colonMode);
		if (!optarg && !argv[optind][shortIndex])
		{
			optind++;
			shortIndex = 1;
		}
		return result;
	}
	else
	{
		return handleLongOption(argv[optind++] + 2, longopts, longindex, colonMode);
	}
}
