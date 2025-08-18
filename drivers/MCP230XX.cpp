#include <MCP230XX.h>

MCP230XX::MCP230XX(uint8_t address,uint8_t chip){
	_address=address;
    _chip=chip;
}

MCP230XX::~MCP230XX(){
	
}