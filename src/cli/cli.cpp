#include "xml_editor/xml.hpp"
#include "xml_editor/io.hpp"
namespace xml_editor::cli {
    std::string run_cli(int argc, char** argv)
    {
        std::string filePath = "";
        std::string outputPath = "";

        if(argv[0]=="xml_editor")
        {
            if(argv[1] == "verify")
            {
                if(argv[2] == "-i")
                {
                    filePath = argv[3];
                    std::string getXml = io::file_read(filePath);
                    if(!getXml.empty())
                    {

                        bool isValid = xml::is_valid(getXml);
                        if(!isValid)
                        {
                            printf("%d Errors Found\n",xml::get_error_count);
                            printf("Errors Log:\n");
                            for (auto&& i : xml::get_errors())
                            {
                                printf("%s\n",i);
                            }

                        }
                        else
                        {
                            printf("Valid File\n");
                        }
                        if(argv[4] == "-f")
                        {
                            if(argv[5] == "-o")
                            {
                                outputPath = argv[6];
                            }
                        }
                    }
                    }
                    if (outputPath != "")
                    {
                        std::string fixedFile = xml::get_fixed_XML();
                        //File writer
                        printf("File corrected at %s\n",outputPath);
                    }

            }

            else if(argv[1] == "format")
            {
                if(argv[2] == "-i")
                {
                    filePath = argv[3];
                    std::string notFormatted = io::file_read(filePath);
                    if (!notFormatted.empty())
                    {
                        Tree* parsed = xml::parse(notFormatted);
                        std::string formatted = xml::format(parsed);
                        if (argv[4] == "-o")
                        {
                            outputPath = argv[5];
                        }
                        if (outputPath != "")
                        {
                            //file writer
                        }
                    }


                }
            }

            else if (argv[1] == "json")
            {
                if(argv[2] == "-i")
                {
                    filePath = argv[3];
                    std::string notFormatted = io::file_read(filePath);
                    if (!notFormatted.empty())
                    {
                        Tree* parsed = xml::parse(notFormatted);
                        std::string formatted = xml::to_json(parsed);
                        if (argv[4] == "-o")
                        {
                            outputPath = argv[5];
                        }
                        if (outputPath != "")
                        {
                            //file writer
                        }
                    }
                }
            }

            else if (argv[1] == "minify")
            {
                if(argv[2] == "-i")
                {
                    filePath = argv[3];
                    std::string notFormatted = io::file_read(filePath);
                    if (!notFormatted.empty())
                    {
                        Tree* parsed = xml::parse(notFormatted);
                        std::string formatted = xml::minify(parsed);
                        if (argv[4] == "-o")
                        {
                            outputPath = argv[5];
                        }
                        if (outputPath != "")
                        {
                            //file writer
                        }
                    }


                }
            }

            else
            {
                printf("Invalid expression\n");
            }

        }

    }
}
