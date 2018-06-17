----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 2018/04/18 16:15:34
-- Design Name: 
-- Module Name: connector - Behavioral
-- Project Name: 
-- Target Devices: 
-- Tool Versions: 
-- Description: 
-- 
-- Dependencies: 
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
-- 
----------------------------------------------------------------------------------


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use ieee.numeric_std.all;
-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity connector is
    Port ( clk : in STD_LOGIC;
           reset : in STD_LOGIC;
           key_read : in STD_LOGIC;
           key_write : in STD_LOGIC;
           s_write : out STD_LOGIC;
           s_read : out STD_LOGIC;
           w_addr : out STD_LOGIC_VECTOR (31 downto 0);
           w_data : out STD_LOGIC_VECTOR (31 downto 0);
           r_addr : out STD_LOGIC_VECTOR (31 downto 0);
           r_data : in STD_LOGIC_VECTOR (31 downto 0);
           w_done : in STD_LOGIC;
           r_done : in STD_LOGIC);
end connector;

architecture Behavioral of connector is
-- write signal 
signal w_issued:std_logic;  ---- mark data and addr already send once
signal inter_key_write:std_logic:='0'; ---- recive key_write signal
signal signal_write:std_logic; ---- send   siganl of write to master 
signal write_addr:std_logic_vector(31 downto 0):=x"00200000";
signal write_data:std_logic_vector(31 downto 0):=x"aa200000";
signal triger_write_tx:std_logic; ---- triger write state machine
signal triger_w_t_1:std_logic;  
signal triger_w_t_2: std_logic;
signal key_write_in: std_logic;  ---- to inter_key_write make NOT operator
type write_state is ( IDLE, W_TRIGER, W_DATA_TX,WAIT_DONE);
signal w_state:write_state;
-- read signal
signal inter_key_read:std_logic:='0';
signal signal_read:std_logic;
signal read_addr:std_logic_vector(31 downto 0):=x"00200000";
signal read_data:std_logic_vector(31 downto 0);
signal triger_read_tx:std_logic;
signal triger_r_t_1:std_logic;
signal triger_r_t_2:std_logic;
signal key_read_in: std_logic;
type read_state is ( IDLE, R_TRIGER,R_DATA_TX, WAIT_DONE);
signal r_state: read_state;
begin
-- read data
---- read pulse generator
triger_read_tx<=( not triger_r_t_2 ) and triger_r_t_1;
process(clk, reset)
begin
    if rising_edge(clk) then
        if reset = '0' then
            triger_r_t_1<='0';
            triger_r_t_2<='0';
        else
            triger_r_t_1<=key_read_in;
            triger_r_t_2<=triger_r_t_1;
         end if;
    end if;
end process;
---- key read 
inter_key_read<=key_read;
process(clk,reset)
begin
  if rising_edge(clk) then
    if reset = '0' then
        key_read_in<='0';
    else
        key_read_in<= not inter_key_read;
    end if;
  end if;
end process;
---- read state
process(clk,reset)
begin
    if rising_edge(clk) then
        if reset = '0' then
            r_state<=IDLE;
        else
            case(r_state) is
                when IDLE=>
                    if triger_read_tx = '1'  then
                        r_state<=R_TRIGER;
                    else 
                        r_state<=IDLE;
                    end if;
               when  R_TRIGER=>
                    if   r_done = '0'  then
                        r_state<=R_DATA_TX;
                    else
                        r_state<=R_TRIGER;
                   end if;
                when  R_DATA_TX=>
                       if   r_done = '1'  then
                           r_state<=WAIT_DONE;
                      else
                           r_state<=R_DATA_TX;
                       end if;
                when WAIT_DONE=>
                    if  signal_read = '0' then 
                        r_state<=IDLE;
                    else
                        r_state<=WAIT_DONE;
                    end if;
               when others=>
                       r_state<=r_state;
           end case;
         end if;
    end if;
end process;
---- read state deal with
r_addr<=read_addr;
s_read<=signal_read;
process(clk,reset)
begin
    if rising_edge(clk) then
        if reset='0' then
            signal_read<='0';
            read_data<=x"00000000";
            read_addr<=x"00200000";
        else
            case (r_state) is
                when IDLE=>
                    read_addr<=read_addr;
                    signal_read<='0';
                when R_TRIGER=>
                    signal_read<='1';
                when  R_DATA_TX=>
                    read_addr<=x"00200000";
                when WAIT_DONE=>
                    read_data<=r_data;
                    signal_read<='0';
                 when others=>
                    read_addr<=read_addr;
                    read_data<=read_data;
                    signal_read<=signal_read;
            end case;
          end if;
    end if;
end process;
--  write data
---- write pluse  generator
triger_write_tx<= (not triger_w_t_2) and triger_w_t_1;
 process(clk,reset)
 begin 
    if rising_edge(clk) then
        if reset = '0' then 
            triger_w_t_1<='0';
            triger_w_t_2<='0';
        else
           triger_w_t_1<=key_write_in;
           triger_w_t_2<=triger_w_t_1;
         end if;
    end if;
end process;
----  key_write
inter_key_write<=key_write;
process(clk,reset)
begin
  if rising_edge(clk) then
    if reset = '0' then
        key_write_in<='0';
    else
        key_write_in<= not inter_key_write;
    end if;
  end if;
end process;
---- write state 
process(clk,reset)
begin
    if rising_edge(clk) then
        if reset = '0' then
            w_state<=IDLE;
        else
            case(w_state) is
                when IDLE=>
                    if triger_write_tx = '1' then
                        w_state<=W_TRIGER;
                    else 
                        w_state<=IDLE;
                    end if;
               when W_TRIGER=>
                      if w_done = '0' then
                           w_state<=W_DATA_TX;
                      else 
                           w_state<=W_TRIGER;
                      end if;
               when  W_DATA_TX=>
                    if  w_done='1'  then
                        w_state<=WAIT_DONE;
                    else
                        w_state<=W_DATA_TX;
                   end if;
                when WAIT_DONE=>
                    if  signal_write = '0' then 
                        w_state<=IDLE;
                    else
                        w_state<=WAIT_DONE;
                    end if;
               when others=>
                       w_state<=w_state;
           end case;
         end if;
    end if;
end process;
---- write state  deal with
w_addr<=write_addr;
w_data<=write_data;
s_write<=signal_write;
process(clk,reset)
begin
    if rising_edge(clk) then
        if reset ='0' then
             w_issued<='0';
             signal_write<='0';
             write_addr<=x"00200000";
             write_data<=x"aa200000";
        else 
            case(w_state) is
                when IDLE=>
                      write_addr<=write_addr;
                      write_data<=write_data;
                      signal_write<='0';
                      w_issued<='0';
               when W_TRIGER=>
                       signal_write<='1';
               when W_DATA_TX=>
                     if w_issued='0' then 
                        write_addr<=x"00200000";
                        write_data<=std_logic_vector(unsigned(write_data)+x"00000001");
                        w_issued<='1';
                     else
                        write_addr<=write_addr;
                        write_data<=write_data;
                        w_issued<=w_issued;
                     end if;
               when WAIT_DONE=>
                        signal_write <= '0';
               when others=>
                        write_addr<=write_addr;
                        write_data<=write_data;
                        signal_write<=signal_write;
            end case;
         end if;
    end if;
end process;

end Behavioral;
