#include <util/delay.h>

#include <MCP230XX.h>
#include <i2c.h>

MCP23008::MCP23008(uint8_t address){
	_address=address;
}

MCP23008::~MCP23008(){

}

void MCP23008::init(){
    // Written only for this project
	write_byte_blocking(MCP_IOCON,(1<<IOCON_SEQOP)|(1<<IOCON_DISSLW));
    write_byte_blocking(MCP_IODIR,0xff);
    write_byte_blocking(MCP_IPOL,0xff);
    write_byte_blocking(MCP_GPINTEN,0xff);
    write_byte_blocking(MCP_DEFVAL,0x00);
    write_byte_blocking(MCP_INTCON,0xff);
    write_byte_blocking(MCP_GPPU,0xff);
    _delay_ms(10);
    i2c_read_reg_from_adddress_blocking(_address,MCP_INTCAP);
}

uint8_t MCP23008::read_byte_blocking(uint8_t reg){
    return i2c_read_reg_from_adddress_blocking(_address,reg);
}

void MCP23008::write_byte_blocking(uint8_t reg,uint8_t val){
    uint8_t data[2]={0};

    data[0]=reg;
    data[1]=val;
    i2c_write_bytes_to_address_blocking(_address,data,2);
}