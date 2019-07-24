#ifndef ELEMENTH
#define ELEMENTH

#include "elementabstract.h"


class Element : public ElementAbstract
{
public:
    Element(const fs::path& path);
    Element(const fs::path& path, const std::string& title, const Tags tags, const bool& pinned=false, const bool& favorited=false);
    Element(const Element& other);

    static ElementsList construct_list_elements(const PathsList& f);
    inline void reload() {  setup( path() );  }

    static bool hasHeader(const fs::path& fi);
    /**
     * get the header of a file, each line in a string
     * and everything in a vector of strings.
     * NOTE: a header is the lines inside two lines of "---"
     */
    static StringList getHeader(const fs::path fi);
    static bool isMD(const fs::path &f);		// verify if the file has a markdown extension
    /**
     * scans all the files of a directory and return the markdown files inside a list of paths
     */
    static PathsList fetch_files(const std::string& dir);

    
    /********************************************************/
    /************  Extract the value in a header line *******/
    /* receives a line of the header of a file, which are
     * written in a key-value fashion.
     * returns the value
     ********************************************************/

    /** Extract the title from a string in this form: "title: this is the title"
     */
    static std::string extract_title(const std::string& tit);

    /**
     * receives : "pinned: true"
     * returns  : bool(true)
     */
    static bool extract_pinned(const std::string& pi);
    static bool extract_favorited(const std::string& fav);
    /* receive this:
     * "tags: [Notebooks/sheets/random, status/infinite, type/all]"
     * return the items between braquets and put them in a vector of strings
     */
    static StringList extract_tags(const std::string& s);

    /**
     * split a single string into particles and return them in a vector of strings
     * for example:
     *      input: "Notebooks/sheets/random"
     *      output: {"Notebooks", "sheets, "random"}
     *
     * use in this way: split("Notebooks/sheets/random", "/");
     *
     * since I have dropped the usage of yaml-cpp, this function
     * is also used to separate the tags block by passing ", " as a delimiter.
     *
     * function found in:
          https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
    */
    static StringList split_single_tag(const std::string& s, const std::string& delimiter="/");
    /**
     * transform the raw tags retrieved from the file,
     * parse them using split_single_tag()
     * and put them inside a vector of vectors of strings (Tags typedef)
     */
    static Tags parse_tags(StringList& raw_tags);
    
    /********************************************************/
    /************  find something in header   ***************/
    /* functions that receives the list of header lines
     * each of them is designed to find a particular line
     ********************************************************/

    /**
     * search in a list of headers (returned by getHeader) for the line "title:"
     * and return the title (using extract_title)
     */
    static std::string find_title_inheader(const StringList& header);
    /**
     * search in a list of headers (returned by getHeader) for the line "tags:"
     * and return the title (using extract_ta)
     */
    static std::string find_tags_inheader(const StringList& header);
    /**
     * search in a list of headers (returned by getHeader) for the line "pinned:"
     * and return the title (using extract_pinned)
     */
    static std::string find_pinned_inheader(const StringList& header);
    /**
     * search in a list of headers (returned by getHeader) for the line "favorited:"
     * and return the title (using extract_favorited)
     */
    static std::string find_favorite_inheader(const StringList& header);

    
    /**
     * remove trailing spaces
     * found in: https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
     */
    static void trim(std::string &s);

private:
    void setup(const fs::path& path);	// construct the object by calling all the appropriate functions
    /**
     * Some lines are surrounded with quotations,
     * call this function on every title to remove them
     */
    static void remove_quotations(std::string& str);
    static void ltrim(std::string &s);
    static void rtrim(std::string &s);
};

bool operator==(const Element& first, const Element& second);
bool operator!=(const Element& first, const Element& second);

#endif // Element_H
