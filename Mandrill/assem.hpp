// assem.hpp
// generates MIPS assembly from the
// intermediate code

#ifndef ASSEM_HPP
#define ASSEM_HPP

void genAssem( std::vector<Instruct>& inter_code, const std::string& out_name, std::vector<SymbolTable>& symbol_tables, SymbolTable& functions );

#endif




