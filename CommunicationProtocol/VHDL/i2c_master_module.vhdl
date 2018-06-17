----------------------------------------------------------------------------------
-- Company:
-- Engineer:
--
-- Create Date: 2018/05/12 21:52:32
-- Design Name:
-- Module Name: i2c_master_module - Behavioral
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
USE ieee.std_logic_unsigned.all;
-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity i2c_master_module is
    Generic(
      cpu_fre:integer:=50000000;
      iic_fre:integer:=100000
    );
    Port(
    clk:in std_logic;
    reset:in std_logic;
    --pc_enable:in std_logic;
    --data:out std_logic_vector(7 downto 0);
    --i2c_ack:out std_logic;
    key:in std_logic;
    scl: out std_logic;
    --sda: inout std_logic;
    sda_in:in std_logic;
    sda_out:out std_logic;
    oe_sda:out std_logic;
    ack_error:out std_logic
    );

end i2c_master_module;

architecture Behavioral of i2c_master_module is
   constant divider:integer:=(cpu_fre/iic_fre);
   constant count_section:integer:=(cpu_fre/iic_fre)/4;
   -- clock generator
   signal scl_clk:std_logic;
   signal prev_scl_clk:std_logic;
   signal data_clk:std_logic;
   signal prev_data_clk:std_logic;
   -- state machine
   type state_machine is (idle,
                          start,
                          device,
                          device_ack,
                          wa,
                          wa_ack,
                          ra,
                          ra_ack,
                          wdata,
                          wdata_ack,
                          restart,
                          device_r,
                          device_r_ack,
                          rdata,
                          rdata_nack,
                          quit);
   signal rw_state:state_machine;
   -- inner scl and sda
   signal inner_scl:std_logic;
   signal inner_sda_out:std_logic;
   signal inner_sda_in:std_logic:='1';
   -- Key Triger
   signal key_in:std_logic;
   signal t_f_0:std_logic:='0';
   signal t_f_1:std_logic:='0';
   signal t_en:std_logic;
   --PC Control
   --signal pc_en:std_logic:='0';
   -- control
   signal task_ok:std_logic;
   signal task_en:std_logic;
   -- data
   signal bit_cnt:std_logic_vector(3 downto 0);
   signal rw_sign:std_logic:='1'; --0 w 1 r;
   signal device_addr_w:std_logic_vector(7 downto 0):=x"D0";
   signal device_addr_r:std_logic_vector(7 downto 0):=x"D1";
   signal write_addr:std_logic_vector(7 downto 0):=x"6B"; --Power Mangement
   signal write_data:std_logic_vector(7 downto 0):=x"01"; -- Reset Value;
   signal read_addr:std_logic_vector(7 downto 0):=x"75"; -- read who am i;
   signal read_data:std_logic_vector(7 downto 0):=x"ff";
   signal data:std_logic_vector(7 downto 0);
   -- error
   signal error:std_logic:='0';
   -- inout signal control
   signal en_out:std_logic:='1';
   --signal sda_in:std_logic;
begin

-- Use Key triger
process(clk,reset,key)
begin
  if reset = '0' then
    key_in<='0';
  elsif rising_edge(clk) then
    key_in<=not key;
  end if;
end process;
process(clk,reset,key_in)
begin
  if reset = '0' then
    t_f_0<='0';
    t_f_1<='0';
  elsif rising_edge(clk) then
    t_f_0<=key_in;
    t_f_1<=t_f_0;
  end if;
end process;
 t_en<=(not t_f_1) and t_f_0;
-- SCL Clock
process(clk,reset)
  variable count: integer range 0 to divider;
begin
  if reset = '0' then
    count:=0;
  elsif rising_edge(clk) then
    prev_scl_clk<=scl_clk;
    if count = divider-1 then
      count:=0;
    else
      count:=count+1;
    end if;
    case count is
      when 0 to count_section-1 =>
          scl_clk<='0';
      when count_section to count_section*2-1 =>
          scl_clk<='0';
      when count_section*2 to count_section*3-1 =>
          scl_clk<='1';
      when  count_section*3 to count_section*4-1 =>
          scl_clk<='1';
      when others=>
          scl_clk<=scl_clk;
    end case;
  end if;
end process;
-- DATA Clock
process(clk,reset)
  variable count: integer range 0 to divider;
begin
  if reset = '0' then
    count:=0;
  elsif rising_edge(clk) then
    prev_data_clk<=data_clk;
    if count = divider-1 then
      count:=0;
    else
      count:=count+1;
    end if;
    case count is
      when 0 to count_section-1 =>
          data_clk<='0';
      when count_section to count_section*2-1 =>
          data_clk<='1';
      when count_section*2 to count_section*3-1 =>
          data_clk<='1';
      when  count_section*3 to count_section*4-1 =>
          data_clk<='0';
      when others=>
          data_clk<=scl_clk;
    end case;
  end if;
end process;
-- read and write state generate
process(clk,reset)
begin
  if reset = '0' then
     rw_state<=idle;
  elsif rising_edge(clk) then
    case rw_state is
      when idle=>
         if (task_en = '1' and scl_clk='0' and prev_scl_clk='1') then
             rw_state<=start;
         else
             rw_state<=idle;
         end if;
      when start=>
         if (scl_clk='0' and prev_scl_clk='1') then
            rw_state<=device;
         else
            rw_state<=start;
         end if;
      when device=>
         if conv_integer(bit_cnt)=8 then
            rw_state<=device_ack;
         else
            rw_state<=device;
         end if;
      when device_ack=>
         if (scl_clk='0' and prev_scl_clk='1' and rw_sign='0' and inner_sda_in='0') then
            rw_state<=wa;
         elsif (scl_clk='0' and prev_scl_clk='1' and rw_sign='1' and inner_sda_in='0') then
            rw_state<=ra;
         else
            rw_state<=device_ack;
         end if;
       when wa=>
         if conv_integer(bit_cnt)=8 then
            rw_state<=wa_ack;
         else
            rw_state<=wa;
         end if;
       when wa_ack=>
         if (scl_clk='0' and prev_scl_clk='1' and inner_sda_in='0') then
            rw_state<=wdata;
         else
            rw_state<=wa_ack;
         end if;
        when wdata=>
         if conv_integer(bit_cnt)=8 then
            rw_state<=wdata_ack;
         else
            rw_state<=wdata;
         end if;
        when wdata_ack=>
         if (scl_clk='0' and prev_scl_clk='1' and inner_sda_in='0') then
           rw_state<=quit;
         else
           rw_state<=wdata_ack;
         end if;
        when ra=>
         if conv_integer(bit_cnt)=8 then
           rw_state<=ra_ack;
         else
           rw_state<=ra;
         end if;
        when ra_ack=>
         if (scl_clk='0' and prev_scl_clk='1' and inner_sda_in='0') then
           rw_state<=restart;
         else
           rw_state<=ra_ack;
         end if;
        when restart=>
         if (scl_clk='0' and prev_scl_clk='1') then
           rw_state<=device_r;
         else
           rw_state<=restart;
         end if;
        when device_r=>
         if conv_integer(bit_cnt)=8 then
           rw_state<=device_r_ack;
         else
           rw_state<=device_r;
         end if;
        when device_r_ack=>
         if (scl_clk='0' and prev_scl_clk='1' and inner_sda_in='0') then
           rw_state<=rdata;
         else
           rw_state<=device_r_ack;
         end if;
        when rdata=>
          if conv_integer(bit_cnt)=8 then
            rw_state<=rdata_nack;
          else
            rw_state<=rdata;
          end if;
        when rdata_nack=>
          if (scl_clk='0' and prev_scl_clk='1') then
            rw_state<=quit;
          else
            rw_state<=rdata_nack;
          end if;
        when quit=>
         if (scl_clk='0' and prev_scl_clk='1') then
           rw_state<=idle;
         else
           rw_state<=quit;
         end if;
    end case;
  end if;
end process;
--SCL generate
scl<=inner_scl;
process(clk,reset,rw_state)
begin
 if reset = '0' then
    inner_scl<='1';
 elsif rising_edge(clk) then
   case rw_state is
     when idle=>
      inner_scl<=prev_scl_clk;
     when start=>
      inner_scl<=prev_scl_clk;
     when restart=>
      inner_scl<=prev_scl_clk;
     when quit=>
      inner_scl<=prev_scl_clk;
     when others=>
      inner_scl<=prev_scl_clk;
   end case;
 end if;
end process;
--SDA generate
-- inout control
process(clk,reset,rw_state)
begin
    if reset = '0'  then
        en_out<='1';
    elsif rising_edge(clk) then
        case rw_state is
            when device_ack=>
                    en_out<='0';
            when wa_ack=>
                    en_out<='0';
            when wdata_ack=>
                    en_out<='0';
            when ra_ack=>
                    en_out<='0';
            when device_r_ack=>
                    en_out<='0';
            when rdata=>
                    en_out<='0';
            when others=>
                    en_out<='1';
        end case;
    end if;
end process;
-- data bit count
sda_out<=inner_sda_out;
oe_sda<=not en_out;
process(clk,reset,rw_state)
begin
  if reset = '0' then
    bit_cnt<=(others=>'0');
 elsif rising_edge(clk) then
   if (rw_state=device or
      rw_state=wa or
      rw_state=wdata or
      rw_state=ra or
      rw_state=device_r or
      rw_state=rdata) then
    if (scl_clk='0' and prev_scl_clk='1') then
        if conv_integer(bit_cnt)=8 then
          bit_cnt<=(others=>'0');
        else
          bit_cnt<=bit_cnt+1;
        end if;
     end if;
   else
     bit_cnt<=(others=>'0');
   end if;
 end if;
end process;
--SDA out signal
process(clk,reset,rw_state)
begin
  if reset = '0' then
     inner_sda_out<='1';
  elsif rising_edge(clk) then
     case rw_state is
       when idle =>
          inner_sda_out<='1';
       when start =>
               if data_clk='1' and prev_scl_clk='0' then
                        inner_sda_out<='1';
               elsif  data_clk='1' and prev_scl_clk='1' then
                        inner_sda_out<='1';
              elsif  data_clk='0' and prev_scl_clk='1' then
                        inner_sda_out<='0';
               else
                        inner_sda_out<=inner_sda_out;
               end if;
       when device =>
          if data_clk='1' and prev_data_clk='0' then
              inner_sda_out<=device_addr_w(7-conv_integer(bit_cnt));
          else
              inner_sda_out<=inner_sda_out;
          end if;
        when device_ack=>
               NULL;
        when wa=>
          if data_clk='1' and prev_data_clk='0' then
              inner_sda_out<=write_addr(7-conv_integer(bit_cnt));
          else
              inner_sda_out<=inner_sda_out;
          end if;
        when wa_ack=>
                NULL;
        when  wdata=>
          if data_clk='1' and prev_data_clk='0' then
              inner_sda_out<=write_data(7-conv_integer(bit_cnt));
          else
              inner_sda_out<=inner_sda_out;
          end if;
        when wdata_ack=>
              NULL;
        when ra=>
          if data_clk='1' and prev_data_clk='0' then
             inner_sda_out<=read_addr(7-conv_integer(bit_cnt));
          else
             inner_sda_out<=inner_sda_out;
          end if;
        when ra_ack=>
            NULL;
        when restart=>
         if data_clk='1' and prev_scl_clk='0' then
                  inner_sda_out<='1';
          elsif  data_clk='1' and prev_scl_clk='1' then
                  inner_sda_out<='1';
          elsif  data_clk='0' and prev_scl_clk='1' then
                  inner_sda_out<='0';
          else
                  inner_sda_out<=inner_sda_out;
          end if;
        when device_r=>
          if data_clk='1' and prev_data_clk='0' then
            inner_sda_out<=device_addr_r(7-conv_integer(bit_cnt));
          else
            inner_sda_out<=inner_sda_out;
          end if;
        when device_r_ack=>
               NULL;
        when rdata=>
                NULL;
        when rdata_nack=>
          if data_clk='1' and prev_data_clk='0' then
             inner_sda_out<='1';
          else
             inner_sda_out<=inner_sda_out;
          end if;
        when  quit=>
           if data_clk='1' and prev_scl_clk='0' then
                inner_sda_out<='0';
           elsif  data_clk='1' and prev_scl_clk='1' then
                inner_sda_out<='0';
           elsif  data_clk='0' and prev_scl_clk='1' then
                inner_sda_out<='1';
           else
               inner_sda_out<=inner_sda_out;
           end if;
        when others=>
            inner_sda_out<=inner_sda_out;
     end case;
  end if;
end process;
--SDA in signal
process(clk,reset,rw_state)
begin
  if reset = '0' then
    inner_sda_in<='1';
 elsif rising_edge(clk) then
    if (rw_state=device_ack or
       rw_state=wa_ack or
       rw_state=wdata_ack or
       rw_state=ra_ack or
       rw_state=device_r_ack) then
       if (data_clk='0' and scl_clk='0') then
           inner_sda_in<='1';
       end if;
       if (data_clk='0' and prev_data_clk='1') then
           inner_sda_in<=sda_in;
       end if;
    elsif rw_state=rdata then
       if (data_clk='0' and scl_clk='0') then
         inner_sda_in<='1';
       end if;
       if (data_clk='0' and prev_data_clk='1') then
         read_data(7-conv_integer(bit_cnt))<=sda_in;
       end if;
    else
       inner_sda_in<=inner_sda_in;
    end if;
  end if;
end process;
-- error
ack_error<=error;
process(clk,reset,rw_state)
begin
  if reset = '0' then
      error<='0';
  elsif rising_edge(clk) then
      case rw_state is
           when device_ack=>
                  if prev_data_clk='0' and prev_scl_clk='1' and inner_sda_in /='0'  then
                        error<='1';
                  else
                        error<=error;
                  end if;
           when wa_ack=>
                   if prev_data_clk='0' and prev_scl_clk='1' and inner_sda_in /='0' then
                          error<='1';
                   else
                          error<=error;
                   end if;
           when wdata_ack=>
                   if prev_data_clk='0' and prev_scl_clk='1' and inner_sda_in /='0' then
                           error<='1';
                  else
                           error<=error;
                  end if;
           when ra_ack=>
                  if prev_data_clk='0' and prev_scl_clk='1' and inner_sda_in /='0'  then
                          error<='1';
                  else
                          error<=error;
                  end if;
           when device_r_ack=>
                  if prev_data_clk='0' and prev_scl_clk='1' and inner_sda_in /='0'  then
                          error<='1';
                  else
                          error<=error;
                  end if;
            when others=>
                 error<='0';
      end case;
   end if;
end process;
-- user control
process(clk,reset,rw_state)
begin
  if reset='0' then
     task_ok<='1';
  elsif rising_edge(clk) then
    case rw_state is
       when quit=>
             task_ok<='0';
       when others=>
             task_ok<='1';
    end case;
  end if;
end process;

process(clk,reset,t_en,rw_state)
variable cnt:integer range 0 to 2;
begin
   if reset = '0' then
       task_en<='0';
  elsif rising_edge(clk) then
   if t_en='1' and rw_state=idle and cnt=0 then
       task_en<='1';
       cnt:=cnt+1;
   elsif rw_state=quit then
       task_en<='0';
   else
       task_en<=task_en;
   end if;
  end if;
end process;
-- data out pc
data<=read_data;
end Behavioral;
