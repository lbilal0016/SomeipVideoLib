

bool isJsonValid(const std::string& jsonString)
{
    try{
        json parsedJson = json::parse(jsonString);
        return true;
    } catch (const json::parse_error& e){
        std::cout << "Json Parsing failed.\n";
        //  JSON parsing failed
        return false;
    }
}




