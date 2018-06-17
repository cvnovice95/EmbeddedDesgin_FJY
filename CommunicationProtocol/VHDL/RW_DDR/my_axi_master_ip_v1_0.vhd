library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity my_axi_master_ip_v1_0 is
	generic (
		-- Users to add parameters here

		-- User parameters ends
		-- Do not modify the parameters beyond this line


		-- Parameters of Axi Master Bus Interface M00_AXI
		C_M00_AXI_START_DATA_VALUE	: std_logic_vector	:= x"AA000000";
		C_M00_AXI_TARGET_SLAVE_BASE_ADDR	: std_logic_vector	:= x"40000000";
		C_M00_AXI_ADDR_WIDTH	: integer	:= 32;
		C_M00_AXI_DATA_WIDTH	: integer	:= 32;
		C_M00_AXI_TRANSACTIONS_NUM	: integer	:= 1
	);
	port (
		-- Users to add ports here
        ----> User  Modify
              m00_axi_user_write_tx: in std_logic;
              m00_axi_user_w_addr:in std_logic_vector(31 downto 0);     
              m00_axi_user_w_data:in std_logic_vector(31 downto 0); 
              m00_axi_user_w_done: out std_logic;
              m00_axi_user_w_state_tx:out std_logic;
              m00_axi_user_w_state_done:out std_logic;     
                      
              m00_axi_user_read_tx: in std_logic;    
              m00_axi_user_r_addr:in std_logic_vector(31 downto 0);
              m00_axi_user_r_data:out std_logic_vector(31 downto 0);    
              m00_axi_user_r_done:out std_logic;
              m00_axi_user_r_state_tx: out std_logic;
              m00_axi_user_r_state_done:out std_logic;       
        ----> End Modify
		-- User ports ends
		-- Do not modify the ports beyond this line


		-- Ports of Axi Master Bus Interface M00_AXI
		m00_axi_init_axi_txn	: in std_logic;
		m00_axi_error	: out std_logic;
		m00_axi_txn_done	: out std_logic;
		m00_axi_aclk	: in std_logic;
		m00_axi_aresetn	: in std_logic;
		m00_axi_awaddr	: out std_logic_vector(C_M00_AXI_ADDR_WIDTH-1 downto 0);
		m00_axi_awprot	: out std_logic_vector(2 downto 0);
		m00_axi_awvalid	: out std_logic;
		m00_axi_awready	: in std_logic;
		m00_axi_wdata	: out std_logic_vector(C_M00_AXI_DATA_WIDTH-1 downto 0);
		m00_axi_wstrb	: out std_logic_vector(C_M00_AXI_DATA_WIDTH/8-1 downto 0);
		m00_axi_wvalid	: out std_logic;
		m00_axi_wready	: in std_logic;
		m00_axi_bresp	: in std_logic_vector(1 downto 0);
		m00_axi_bvalid	: in std_logic;
		m00_axi_bready	: out std_logic;
		m00_axi_araddr	: out std_logic_vector(C_M00_AXI_ADDR_WIDTH-1 downto 0);
		m00_axi_arprot	: out std_logic_vector(2 downto 0);
		m00_axi_arvalid	: out std_logic;
		m00_axi_arready	: in std_logic;
		m00_axi_rdata	: in std_logic_vector(C_M00_AXI_DATA_WIDTH-1 downto 0);
		m00_axi_rresp	: in std_logic_vector(1 downto 0);
		m00_axi_rvalid	: in std_logic;
		m00_axi_rready	: out std_logic
	);
end my_axi_master_ip_v1_0;

architecture arch_imp of my_axi_master_ip_v1_0 is

	-- component declaration
	component my_axi_master_ip_v1_0_M00_AXI is
		generic (
		C_M_START_DATA_VALUE	: std_logic_vector	:= x"AA000000";
		C_M_TARGET_SLAVE_BASE_ADDR	: std_logic_vector	:= x"40000000";
		C_M_AXI_ADDR_WIDTH	: integer	:= 32;
		C_M_AXI_DATA_WIDTH	: integer	:= 32;
		C_M_TRANSACTIONS_NUM	: integer	:= 1
		);
		port (
		 ----> User  Modify
               USER_WRITE_TX: in std_logic;
               USER_W_ADDR:in std_logic_vector(31 downto 0);
               USER_W_DATA:in std_logic_vector(31 downto 0);
               USER_W_DONE: out std_logic;
               
               USER_W_STATE_TX: out std_logic;
               USER_W_STATE_DONE:out std_logic;
               
               USER_READ_TX:in std_logic;
               USER_R_ADDR:in std_logic_vector(31 downto 0);
               USER_R_DATA:out std_logic_vector(31 downto 0);
               USER_R_DONE:out std_logic;
               
               USER_R_STATE_TX: out std_logic;
               USER_R_STATE_DONE:out std_logic;
            
               ----> End Modify
		INIT_AXI_TXN	: in std_logic;
		ERROR	: out std_logic;
		TXN_DONE	: out std_logic;
		M_AXI_ACLK	: in std_logic;
		M_AXI_ARESETN	: in std_logic;
		M_AXI_AWADDR	: out std_logic_vector(C_M_AXI_ADDR_WIDTH-1 downto 0);
		M_AXI_AWPROT	: out std_logic_vector(2 downto 0);
		M_AXI_AWVALID	: out std_logic;
		M_AXI_AWREADY	: in std_logic;
		M_AXI_WDATA	: out std_logic_vector(C_M_AXI_DATA_WIDTH-1 downto 0);
		M_AXI_WSTRB	: out std_logic_vector(C_M_AXI_DATA_WIDTH/8-1 downto 0);
		M_AXI_WVALID	: out std_logic;
		M_AXI_WREADY	: in std_logic;
		M_AXI_BRESP	: in std_logic_vector(1 downto 0);
		M_AXI_BVALID	: in std_logic;
		M_AXI_BREADY	: out std_logic;
		M_AXI_ARADDR	: out std_logic_vector(C_M_AXI_ADDR_WIDTH-1 downto 0);
		M_AXI_ARPROT	: out std_logic_vector(2 downto 0);
		M_AXI_ARVALID	: out std_logic;
		M_AXI_ARREADY	: in std_logic;
		M_AXI_RDATA	: in std_logic_vector(C_M_AXI_DATA_WIDTH-1 downto 0);
		M_AXI_RRESP	: in std_logic_vector(1 downto 0);
		M_AXI_RVALID	: in std_logic;
		M_AXI_RREADY	: out std_logic
		);
	end component my_axi_master_ip_v1_0_M00_AXI;

begin

-- Instantiation of Axi Bus Interface M00_AXI
my_axi_master_ip_v1_0_M00_AXI_inst : my_axi_master_ip_v1_0_M00_AXI
	generic map (
		C_M_START_DATA_VALUE	=> C_M00_AXI_START_DATA_VALUE,
		C_M_TARGET_SLAVE_BASE_ADDR	=> C_M00_AXI_TARGET_SLAVE_BASE_ADDR,
		C_M_AXI_ADDR_WIDTH	=> C_M00_AXI_ADDR_WIDTH,
		C_M_AXI_DATA_WIDTH	=> C_M00_AXI_DATA_WIDTH,
		C_M_TRANSACTIONS_NUM	=> C_M00_AXI_TRANSACTIONS_NUM
	)
	port map (
	    USER_WRITE_TX=>m00_axi_user_write_tx,
        USER_W_ADDR=>m00_axi_user_w_addr,
        USER_W_DATA=>m00_axi_user_w_data,
        USER_W_DONE=>  m00_axi_user_w_done,
        USER_W_STATE_TX=>m00_axi_user_w_state_tx,
        USER_W_STATE_DONE=>m00_axi_user_w_state_done,
        USER_READ_TX=>m00_axi_user_read_tx,
        USER_R_ADDR=>m00_axi_user_r_addr,
        USER_R_DATA=>m00_axi_user_r_data,
        USER_R_DONE=>m00_axi_user_r_done,
        USER_R_STATE_TX=>m00_axi_user_r_state_tx,
        USER_R_STATE_DONE=>m00_axi_user_r_state_done,
        
		INIT_AXI_TXN	=> m00_axi_init_axi_txn,
		ERROR	=> m00_axi_error,
		TXN_DONE	=> m00_axi_txn_done,
		M_AXI_ACLK	=> m00_axi_aclk,
		M_AXI_ARESETN	=> m00_axi_aresetn,
		M_AXI_AWADDR	=> m00_axi_awaddr,
		M_AXI_AWPROT	=> m00_axi_awprot,
		M_AXI_AWVALID	=> m00_axi_awvalid,
		M_AXI_AWREADY	=> m00_axi_awready,
		M_AXI_WDATA	=> m00_axi_wdata,
		M_AXI_WSTRB	=> m00_axi_wstrb,
		M_AXI_WVALID	=> m00_axi_wvalid,
		M_AXI_WREADY	=> m00_axi_wready,
		M_AXI_BRESP	=> m00_axi_bresp,
		M_AXI_BVALID	=> m00_axi_bvalid,
		M_AXI_BREADY	=> m00_axi_bready,
		M_AXI_ARADDR	=> m00_axi_araddr,
		M_AXI_ARPROT	=> m00_axi_arprot,
		M_AXI_ARVALID	=> m00_axi_arvalid,
		M_AXI_ARREADY	=> m00_axi_arready,
		M_AXI_RDATA	=> m00_axi_rdata,
		M_AXI_RRESP	=> m00_axi_rresp,
		M_AXI_RVALID	=> m00_axi_rvalid,
		M_AXI_RREADY	=> m00_axi_rready
	);

	-- Add user logic here

	-- User logic ends

end arch_imp;
