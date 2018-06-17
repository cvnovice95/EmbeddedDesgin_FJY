----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    23:09:05 09/19/2016 
-- Design Name: 
-- Module Name:    SevenSegment - Behavioral 
-- Project Name: 
-- Target Devices: 
-- Tool versions: 
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
use IEEE.STD_LOGIC_UNSIGNED.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity SevenSegment is
    Port ( CLK : in  STD_LOGIC;
           RESET : in  STD_LOGIC;
           LED_BUS : out  STD_LOGIC_VECTOR (7 downto 0);
           DATA_L : out  STD_LOGIC;
           LED_L : out  STD_LOGIC);
end SevenSegment;

architecture Behavioral of SevenSegment is
signal counter_timer:std_logic_vector(26 downto 0);
signal LED_bus_t:std_logic_vector(71 downto 0):="011011110111111100000111011111010110110101100110010011110101101100000110";
signal Student_Number:std_logic_vector(63 downto 0):="0101101100111111000001100110011000000110000001110110011001111101";
signal data_l_t:std_logic:='0';
signal led_l_t:std_logic:='0';
signal send_data:std_logic_vector(7 downto 0):="00000000";
shared variable count_i:integer:=0;
signal Positon_LED:std_logic_vector(63 downto 0):="0111111110111111110111111110111111110111111110111111110111111110";
constant interval:integer:=50000;
constant interval_d:integer:=50000000;
begin
--time_count
process(CLK,RESET)
begin
   if RESET = '0' then
		counter_timer <= (others =>'0');
	elsif falling_edge(CLK) then
	   if conv_integer(counter_timer)=interval_d-1 then--8*interval-1 then    --interval_d-1
			counter_timer <=(others=>'0');
		else
			counter_timer <=counter_timer+1;
		end if;
	end if;
end process;
--update data
process(CLK,RESET)
begin
	if falling_edge(CLK) then
	 if conv_integer(counter_timer)=6 then
	    if count_i=9 then
			count_i:=1;
		else
			count_i:=count_i+1;
		end if;
	 end if;
	end if;
end process;

process(CLK)
begin
	if falling_edge(CLK) then
	 case conv_integer(counter_timer) is
	   when 0 to 2 =>
			data_l_t <='0';
			led_l_t<='1';
			send_data<="00000000";
	   when 3 to 5 =>
			data_l_t <='1';
			led_l_t<='0';
		when 7 to interval_d-1 =>
		   send_data<=LED_bus_t(count_i*8-1 downto count_i*8-8);
		when others =>
		   send_data<=send_data;
	 end case;
	end if;
end process;

--process(CLK)
--begin
--  if falling_edge(CLK) then
--	  case conv_integer(counter_timer) is
--	 
--	    when  0+interval*0 to 100+interval*0=>
--				data_l_t <='0';
--				led_l_t<='1';
--				send_data<=Positon_LED((0+1)*8-1 downto (0+1)*8-8);
--		 when 101+interval*0 to 200+interval*0 =>
--				data_l_t <='1';
--				led_l_t<='0';
--	    when 201+interval*0 to (0+1)*interval-1=>
--				send_data<=Student_Number( 63-0*8 downto 56-0*8);
--		 
--		 when  0+interval*1 to 100+interval*1=>
--				data_l_t <='0';
--				led_l_t<='1';
--				send_data<=Positon_LED((1+1)*8-1 downto (1+1)*8-8);
--		 when 101+interval*1 to 200+interval*1 =>
--				data_l_t <='1';
--				led_l_t<='0';
--	    when 201+interval*1 to (1+1)*interval-1=>
--				send_data<=Student_Number( 63-1*8 downto 56-1*8);
--			  
--		 when  0+interval*2 to 100+interval*2=>
--				data_l_t <='0';
--				led_l_t<='1';
--				send_data<=Positon_LED((2+1)*8-1 downto (2+1)*8-8);
--		 when 101+interval*2 to 200+interval*2 =>
--				data_l_t <='1';
--				led_l_t<='0';
--	    when 201+interval*2 to (2+1)*interval-1=>
--				send_data<=Student_Number( 63-2*8 downto 56-2*8);
--			  
--		 when  0+interval*3 to 100+interval*3=>
--				data_l_t <='0';
--				led_l_t<='1';
--				send_data<=Positon_LED((3+1)*8-1 downto (3+1)*8-8);
--		 when 101+interval*3 to 200+interval*3 =>
--				data_l_t <='1';
--				led_l_t<='0';
--	    when 201+interval*3 to (3+1)*interval-1=>
--				send_data<=Student_Number( 63-3*8 downto 56-3*8);
--			  
--		 when  0+interval*4 to 100+interval*4=>
--				data_l_t <='0';
--				led_l_t<='1';
--				send_data<=Positon_LED((4+1)*8-1 downto (4+1)*8-8);
--		 when 101+interval*4 to 200+interval*4 =>
--				data_l_t <='1';
--				led_l_t<='0';
--	    when 201+interval*4 to (4+1)*interval-1=>
--				send_data<=Student_Number( 63-4*8 downto 56-4*8);
--			  
--		 when  0+interval*5 to 100+interval*5=>
--				data_l_t <='0';
--				led_l_t<='1';
--				send_data<=Positon_LED((5+1)*8-1 downto (5+1)*8-8);
--		 when 101+interval*5 to 200+interval*5 =>
--				data_l_t <='1';
--				led_l_t<='0';
--	    when 201+interval*5 to (5+1)*interval-1=>
--		     send_data<=Student_Number( 63-5*8 downto 56-5*8);
--			  
--		 when  0+interval*6 to 100+interval*6=>
--				data_l_t <='0';
--				led_l_t<='1';
--				send_data<=Positon_LED((6+1)*8-1 downto (6+1)*8-8);
--		 when 101+interval*6 to 200+interval*6 =>
--				data_l_t <='1';
--				led_l_t<='0';
--	    when 201+interval*6 to (6+1)*interval-1=>
--		      send_data<=Student_Number( 63-6*8 downto 56-6*8);
--			  
--		 when  0+interval*7 to 100+interval*7=>
--				data_l_t <='0';
--				led_l_t<='1';
--				send_data<=Positon_LED((7+1)*8-1 downto (7+1)*8-8);
--		 when 101+interval*7 to 200+interval*7 =>
--				data_l_t <='1';
--				led_l_t<='0';
--	    when 201+interval*7 to (7+1)*interval-1=>
--				send_data<=Student_Number( 63-7*8 downto 56-7*8);
--		 when others =>
--		     send_data<=send_data;
--		end case;
--	end if;
--end process;
DATA_L <=data_l_t;
LED_L <=led_l_t;
LED_BUS <=send_data;
end Behavioral;

