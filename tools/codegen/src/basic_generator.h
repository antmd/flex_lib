#ifndef BASIC_GENERATOR_H
#define BASIC_GENERATOR_H

#include "generator_base.h"
#include "cpp_source_stream.h"

namespace codegen 
{
class BasicGenerator : public GeneratorBase
{
public:
    BasicGenerator(const Options& opts);
    
    // GeneratorBase interface
    void OnCompilationStarted() override;
    bool Validate() override;
    bool GenerateOutput() override;
    
protected:
    const Options& m_options;
    
    virtual void WriteHeaderPreamble(CppSourceStream& hdrOs) {}
    virtual void WriteHeaderContent(CppSourceStream& hdrOs) {}
    virtual void WriteHeaderPostamble(CppSourceStream& hdrOs) {}

    virtual void WriteSourcePreamble(CppSourceStream& srcOs) {}
    virtual void WriteSourceContent(CppSourceStream& srcOs) {}
    virtual void WriteSourcePostamble(CppSourceStream& srcOs) {}
};

} // codegen

#endif // BASIC_GENERATOR_H
