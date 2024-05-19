#include <iostream>
#include <fstream>
#include <regex>
#include <filesystem>
#include <algorithm>

using namespace std;
namespace fs = std::filesystem;

// Function to convert a string to uppercase
string toUppercase(string str) {
    transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

// Function to search for a string matching the regex pattern at a given offset in a file
string searchStringAtOffset(const string& filePath, long offset, const string& regexPattern) {
    try {
        ifstream file(filePath, ios::binary);
        if (!file.is_open()) {
            cout << "Unable to open file: " << filePath << endl;
            exit(EXIT_FAILURE); // Exit if unable to open file
        }

        file.seekg(offset);
        const int chunkSize = 4096; // Chunk size of 4 KB
        char buffer[chunkSize];
        string content;

        regex pattern(regexPattern);
        smatch match;

        int totalBytesRead = 0;
        while (file.read(buffer, sizeof(buffer))) {
            totalBytesRead += file.gcount();

            content.append(buffer, file.gcount());

            // Search for the pattern in the current content
            if (regex_search(content, match, pattern)) {
                return match.str();
            }

            // Clear the content to avoid infinite memory growth
            content.clear();

            // Check if the total number of bytes read exceeds 10 MB
            if (totalBytesRead >= 10 * 100000) {
                break;
            }
        }

        // Search for the pattern in the remaining content
        if (regex_search(content, match, pattern)) {
            return match.str();
        }
    }
    catch (const exception& ex) {
        cout << "An error occurred while reading the file: " << ex.what() << endl;
    }

    return "";
}

void displayHelp(const string& programName) {
    cout << "Usage: " << programName << " <path_to_file> [-1] [-2] [-3] [-help]" << endl;
    cout << "Options:" << endl;
    cout << "  -1  Format: SLUS-123.45" << endl;
    cout << "  -2  Format: SLUS-12345" << endl;
    cout << "  -3  Format: SLUS12345" << endl;
    cout << "  -help" << endl;
}

int main(int argc, char* argv[]) {
    if (argc < 2 || argc > 3 || (argc == 2 && string(argv[1]) == "-help")) {
        displayHelp(argv[0]);
        return 0;
    }

    string filePath = argv[1];
    bool FormatWithUnderscoreAndDot = (argc == 3 && string(argv[2]) == "-1");
    bool RemoveDotsOnly = (argc == 3 && string(argv[2]) == "-2");
    bool RemoveUnderscoresAndDots = (argc == 3 && string(argv[2]) == "-3");

    string extension = fs::path(filePath).extension().string();
    extension = toUppercase(extension);

    string regexPattern = "[A-Z]{4}[_-][0-9]{3}[.][0-9]{2}|LSP[0-9]{5}[.][0-9]{3}";

    // Default offset for .BIN files
    long offset = 51380;

    // Change the offset for .VCD files. PS2 POPS
    if (extension == ".VCD") {
        offset = 1100032;
    }

    // Change the offset for .PBP & PKG files. PSP, PS3
    if (extension == ".PBP" || extension == ".PKG") {
        offset = 0;
        regexPattern = "[A-Z]{4}[0-9]{5}";
    }

    string searchString = searchStringAtOffset(filePath, offset, regexPattern);
    if (searchString.empty()) {

        // Change the offset for .BIN files.
        if (extension == ".BIN") {
            offset = 37500;
        }

        // Change the offset for .VCD files. PS2 POPS
        if (extension == ".VCD") {
            offset = 1086160;
        }

        // Below are some game exceptions that do not have a system.cnf, their ID is assigned either based on a unique file or the disc label.
        string alternativeRegexPattern = "MEDAROT_R|DRAGONWARRIOR7_1|DRAGONWARRIOR7_2|ROAD WRITER|DEJIKURO|PACHIOKUN|SLPS_00137|LADIES\\.DA;1|BANNY\\.VDF;1|NP_ISHI0\\.SPL;1|G9_100L\\.DA;1|MYST\\.CCS;1|ATLUS\\.STR;1|ATKDAT\\.CFL;1|RX78\\.TMD;1|_HA30\\.STR;1|A_LOGO\\.STR;1|ENTSTAR\\.STR;1|KAMI00\\.BIN;1|DEGI3\\.STR;1|LOG\\.BAK;1|WIN_FNT2\\.BIN;1|PACHI2BG\\.TIM;1|RDATWI\\.BIN;1|AH1MAP\\.MAP;1|TORNYA\\.TPC;1|EPI_MCH\\.STR;1|H_TAISOH\\.VHB;1|OBJ\\.RRO;1|VTENNIS\\.STR;1|QUE184\\.TIM;1|GOACC\\.TIM;1|OPENXA\\.STR;1";
        searchString = searchStringAtOffset(filePath, offset, alternativeRegexPattern);

        // Based on Disc Label Name
        if (searchString == "MEDAROT_R") { searchString = "SLPS_024.14"; }
        if (searchString == "DRAGONWARRIOR7_1") { searchString = "SLUS_012.06"; }
        if (searchString == "DRAGONWARRIOR7_2") { searchString = "SLUS_013.46"; }
        if (searchString == "ROAD WRITER") { searchString = "907127.001"; }
        if (searchString == "DEJIKURO") { searchString = "SLPS_005.49"; }
        if (searchString == "PACHIOKUN") { searchString = "SLPS_000.37"; }
        if (searchString == "SLPS_00137") { searchString = "SLPS_001.37"; }

        // Based on File Name
        if (searchString == "LADIES.DA;1") { searchString = "SLPS_000.23"; }
        if (searchString == "BANNY.VDF;1") { searchString = "SLPS_000.04"; }
        if (searchString == "NP_ISHI0.SPL;1") { searchString = "SLPS_001.13"; }
        if (searchString == "G9_100L.DA;1") { searchString = "SLPS_000.38"; }
        if (searchString == "MYST.CCS;1") { searchString = "SLPS_000.24"; }
        if (searchString == "ATLUS.STR;1") { searchString = "SLPS_001.04"; }
        if (searchString == "ATKDAT.CFL;1") { searchString = "SCPS_100.02"; }
        if (searchString == "RX78.TMD;1") { searchString = "SLPS_000.35"; }
        if (searchString == "_HA30.STR;1") { searchString = "SCPS_100.16"; }
        if (searchString == "A_LOGO.STR;1") { searchString = "SLPS_000.85"; }
        if (searchString == "ENTSTAR.STR;1") { searchString = "SLPS_000.22"; }
        if (searchString == "KAMI00.BIN;1") { searchString = "SLPS_000.89"; }
        if (searchString == "LOG.BAK;1") { searchString = "SLPS_000.51"; }
        if (searchString == "WIN_FNT2.BIN;1") { searchString = "SLPS_000.90"; }
        if (searchString == "PACHI2BG.TIM;1") { searchString = "SLPS_000.21"; }
        if (searchString == "RDATWI.BIN;1") { searchString = "SLPS_000.37"; }
        if (searchString == "AH1MAP.MAP;1") { searchString = "SLPS_000.60"; }
        if (searchString == "TORNYA.TPC;1") { searchString = "SLPS_000.92"; }
        if (searchString == "EPI_MCH.STR;1") { searchString = "SCPS_100.09"; }
        if (searchString == "OBJ.RRO;1") { searchString = "SLPS_000.01"; }
        if (searchString == "VTENNIS.STR;1") { searchString = "SLPS_001.03"; }
        if (searchString == "QUE184.TIM;1") { searchString = "SLPS_000.07"; }
        if (searchString == "GOACC.TIM;1") { searchString = "SLPS_000.48"; }
        if (searchString == "OPENXA.STR;1") { searchString = "SLPS_000.65"; }
    }

    if (!searchString.empty()) {
        if (FormatWithUnderscoreAndDot) {
            // Force the format is "SLUS-123.45"
            if (regex_match(searchString, regex("[A-Z]{4}[0-9]{5}"))) {
                searchString.insert(4, "_");
                searchString.insert(8, ".");
            }
        }

        if (RemoveDotsOnly) {
            // Ensure the format is "SLUS-12345"
            searchString.erase(remove(searchString.begin(), searchString.end(), '.'), searchString.end());
            searchString.erase(remove(searchString.begin(), searchString.end(), '_'), searchString.end());
            searchString.insert(4, "-");
        }
        if (RemoveUnderscoresAndDots) {
            // Ensure the format is "SLUS12345"
            searchString.erase(remove(searchString.begin(), searchString.end(), '_'), searchString.end());
            searchString.erase(remove(searchString.begin(), searchString.end(), '.'), searchString.end());
        }

        cout << searchString << endl;
    }

    return 0;
}
