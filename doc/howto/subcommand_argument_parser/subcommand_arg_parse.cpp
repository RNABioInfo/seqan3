#include <seqan3/argument_parser/all.hpp>

using namespace seqan3;

// =====================================================================================================================
// pull
// =====================================================================================================================

struct pull_arguments
{
    std::string repository{};
    std::string branch{};
    bool progress{false};
};

int run_git_pull(argument_parser & parser)
{
    pull_arguments args{};

    parser.add_positional_option(args.repository, "The repository name to pull from.");
    parser.add_positional_option(args.branch, "The branch name to pull from.");

    try
    {
        parser.parse();
    }
    catch (parser_invalid_argument const & ext)
    {
        debug_stream << "[Error git pull] " << ext.what() << "\n";
        return -1;
    }

    debug_stream << "Git pull with repository " << args.repository << " and branch " << args.branch << std::endl;

    return 0;
}

// =====================================================================================================================
// push
// =====================================================================================================================

struct push_arguments
{
    std::string repository{};
    std::vector<std::string> branches{};
    bool push_all{false};
};

int run_git_push(argument_parser & parser)
{
    push_arguments args{};

    parser.add_positional_option(args.repository, "The repository name to push to.");
    parser.add_positional_option(args.branches, "The branch names to push (if none are given, push current).");

    try
    {
        parser.parse();
    }
    catch (parser_invalid_argument const & ext)
    {
        debug_stream << "[Error git push] " << ext.what() << "\n";
        return -1;
    }

    debug_stream << "Git push with repository " << args.repository << " and branches " << args.branches << std::endl;

    return 0;
}

// =====================================================================================================================
// main
// =====================================================================================================================

int main(int argc, char const ** argv)
{
    //![construction]
    argument_parser top_level_parser{"mygit", argc, argv, true, {"push", "pull"}};
    //![construction]

    // Add information and flags to your top-level parser just as you would do with a normal one.
    // Note that all flags directed at the top-level parser must be specified BEFORE the subcommand key word.
    // Because of ambiguity, we do not allow any (positional) options for the top-level parser.
    top_level_parser.info.description.push_back("You can push or pull from a remote repository.");
    bool flag{false};
    top_level_parser.add_flag(flag, 'f', "flag", "some flag");

    try
    {
        top_level_parser.parse(); // trigger command line parsing
    }
    catch (parser_invalid_argument const & ext) // catch user errors
    {
        debug_stream << "[Error] " << ext.what() << "\n"; // customise your error message
        return -1;
    }

    //![get_sub_parser]
    argument_parser & sub_parser = top_level_parser.get_sub_parser(); // hold a reference to the sub_parser
    //![get_sub_parser]

    std::cout << "Proceed to sub parser." << std::endl;

    if (sub_parser.info.app_name == std::string_view{"mygit-pull"})
        run_git_pull(sub_parser);
    else if (sub_parser.info.app_name == std::string_view{"mygit-push"})
        run_git_push(sub_parser);
    else
        throw std::logic_error{"I do not know sub parser " + sub_parser.info.app_name};
}