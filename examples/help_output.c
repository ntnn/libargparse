#include <argparse.h>
#include <stdio.h>

int main(int argc, char **argv) {
    args *args = args_new();

    args_add_option(args, option_new("f", "feature", "description of feature"));
    args_help(args, stdout);

    args_free(args);
}
