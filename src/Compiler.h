#ifndef COMPILER_H_
#define COMPILER_H_
#include <filesystem>

#include "parser/parser.h"
#include "parser/lex.h"
#include "channels/RulePool.h"
#include "channels/rule.h"
#include "conditional/ConditionalParser.h"
namespace kdl
{



///
/// PURPOSE
///		Manages Compilation
///		
///		Pipeline goes:
///		
///		+-+-+-+-+-+-+-+-+
///		|  Source Code  |		 Text
///		+-+-+-+-+-+-+-+-+		  ┊ 
///			    ↓			      ┊ 
///		+-+-+-+-+-+-+-+-+		  ↓	
///		|     Lexer     |	   CTokenList		 src/parser/lex.h
///		+-+-+-+-+-+-+-+-+         ┊ 
///			    ↓				  ┊ 
///		+-+-+-+-+-+-+-+-+         ↓
///		|   Combinator  |	  RuleBlocks		src/parser/Combinator.h
///		+-+-+-+-+-+-+-+-+         ┊ 
///			    ↓				  ┊ 
///		+-+-+-+-+-+-+-+-+		  ↓
///		|  Rule Parser  |		 Rule			src/parser/RuleParser.h
///		+-+-+-+-+-+-+-+-+         ┊ 
///			    ↓                 ┊ 
///		+-+-+-+-+-+-+-+-+		  ↓
///		|   CTree Gen   |   ConditionalTree		src/conditional/ConditionalParser.h
///		+-+-+-+-+-+-+-+-+	      ┊ 	
///			    ↓			      ┊ 
///		+-+-+-+-+-+-+-+-+	      ┊ 
///		|   Reference   |	      ┊ 			Ssrc/conditional/ConditionalReferenceValidator.h
///     |   Validator   |	      ┊ 
///		+-+-+-+-+-+-+-+-+	      ┊ 
///			    ↓			      ┊ 
///		+-+-+-+-+-+-+-+-+		  ↓
///		|    Code Gen   |		Output			src/backend/<generator>/<generator-code>.h
///		+-+-+-+-+-+-+-+-+
///
class Compiler
{
public:
    Compiler();
    ~Compiler();

    bool compileFile(const char* filename);
    bool compileSource(const char* source, int len);


    void writeFiles(const std::string& directory);

    void dumpRules();

private:
    RulePool m_rules;


    std::vector<std::string> m_inputFiles;
    std::unordered_map<std::string, std::string> m_outputFiles;

    void writeFile(const std::string& name, const std::string& source);
    const char* m_currentFile;
};


} // kdl


#endif // COMPILER_H_