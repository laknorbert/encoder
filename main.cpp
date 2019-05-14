#include "fileenumerator.h"

int main(int argc, char* argv[])
{
    // Program needs 1 parameter (search directory)
    // ToDo: some error indicator, usage display
    if (2 != argc)
        exit(-1);

    // Get the ball rolling
    FileEnumerator fileEnumerator;
    fileEnumerator.run(argv[1]);

    return 0;
}

