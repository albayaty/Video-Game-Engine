-- ==============================================
-- Copyright ©  2014  Ali M. Al-Bayaty
-- 
-- Video-Game-Engine is free software: you can redistribute it and/or modify
-- it under the terms of the GNU General Public License as published by
-- the Free Software Foundation, either version 3 of the License, or
-- any later version.
-- 
-- Video-Game-Engine is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- GNU General Public License for more details.
-- 
-- You should have received a copy of the GNU General Public License
-- along with this program.  If not, see <http://www.gnu.org/licenses/>.
-- 
-- ==============================================
-- 
-- Video Game Engine Project
-- ( EDK: NES Controller VHDL )
-- 
-- MSEE student:    Ali M. Al-Bayaty
-- EE659: System-On-Chip
-- Personal website: <http://albayaty.github.io/>
-- Source code link: <https://github.com/albayaty/Video-Game-Engine.git>
-- 
-- ==============================================
-- 
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use ieee.std_logic_unsigned.all;
use ieee.std_logic_arith.all;
use ieee.numeric_std.all;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity nes_controller is
Port ( reset  : in STD_LOGIC;
		 clk_50 : in STD_LOGIC;
		 led    : out STD_LOGIC_VECTOR (7 downto 0):= "00000000";
		 --nes_latch : inout STD_LOGIC;
		 nes_latch : out STD_LOGIC;
		 --nes_clk   : inout STD_LOGIC;
		 nes_clk   : out STD_LOGIC;
		 nes_data  : in STD_LOGIC);
end nes_controller;

architecture Behavioral of nes_controller is

	signal pulses: integer := 1;
	signal neslatch, nesclk: std_logic;
	
	type statetype is (state1, state2);
	signal currentstate, nextstate : statetype;

begin
	
	-- Generating 12us width signal (nes_latch), and 6us duty cycle (nes_clk)
	-- Sys clk= 50mhz = 20ns = 0.02 us
	-- Latch clk = sys clk * 600 iterations (for high)
	-- Pulses clk = sys clk * 300 and another 300 iterations (for high and low)
	process(clk_50, reset)
		variable counter: integer;
		variable nes_latch_gen: std_logic;
	   variable nes_clk_gen: std_logic;
	begin
		if ( reset = '1') then	
			nes_latch <= '0';
			neslatch  <= '0';
			nes_latch_gen := '1';
			
			nes_clk <= '0';
			nesclk  <= '0';
			nes_clk_gen := '0';
			counter := 1;			
			pulses <= 1;
			
		elsif (rising_edge(clk_50)) then					
			
			-- Generating the nes_latch:
			if ( nes_latch_gen = '1' ) then
				if ( counter <= 600 ) then
					nes_latch <= '1';
					neslatch  <= '1';
					counter := counter + 1;
			
				elsif ( counter > 600 and counter <= 900 ) then
					nes_latch <= '0';
					neslatch  <= '0';
					counter := counter + 1;
			
				elsif ( counter > 900 ) then
					nes_latch <= '0';
					neslatch  <= '0';
					nes_latch_gen := '0';
					nes_clk_gen := '1';
					counter := 1;
					--pulses <= 1;
				
				end if;
			
			-- Generating the nes_clk:
			elsif ( nes_clk_gen = '1' ) then
				if ( counter <= 300 ) then
					nes_clk <= '1';
					nesclk  <= '1';
					counter := counter + 1;
			
				elsif ( counter > 300 and counter <= 600 ) then
					nes_clk <= '0';
					nesclk  <= '0';
					counter := counter + 1;			
			
				elsif ( counter > 600 and pulses <= 7 ) then
					counter := 1;				
					pulses <= pulses + 1;
			
				elsif ( pulses > 7 ) then
					nes_clk <= '0';
					nesclk  <= '0';
					nes_latch_gen := '1';
					nes_clk_gen := '0';
					counter := 1;
					pulses <= 1;
				end if;	
			
			end if;	
			
		end if;
		
	end process;
	
	-- Starting of the FSM:
	cst: process(clk_50, reset)
	begin
		if ( reset = '1') then
			currentstate <= state1;
		
		elsif (rising_edge(clk_50)) then
			currentstate <= nextstate;
		end if;
	end process;
	
	
	nst: process(currentstate, nes_data, neslatch, nesclk, pulses)
	begin
		
		case currentstate is
			when state1 =>
				if ( neslatch = '1' ) then
					led(7) <= not nes_data;
					nextstate <= state2;
				else
					led(7) <= '0';
					nextstate <= state2;
				end if;				
				--nextstate <= state2;
				
			when state2 =>
			   --nextstate <= state2;
				if ( nesclk = '1' and pulses = 1 ) then
					led(6) <= not nes_data;
					nextstate <= state2;
				elsif ( nesclk = '1' and pulses = 2 ) then
					led(5) <= not nes_data;	
					nextstate <= state2;
				elsif ( nesclk = '1' and pulses = 3 ) then
					led(4) <= not nes_data;	
					nextstate <= state2;
				elsif ( nesclk = '1' and pulses = 4 ) then
					led(3) <= not nes_data;
					nextstate <= state2;
				elsif ( nesclk = '1' and pulses = 5 ) then
					led(2) <= not nes_data;	
					nextstate <= state2;
				elsif ( nesclk = '1' and pulses = 6 ) then
					led(1) <= not nes_data;	
					nextstate <= state2;
				elsif ( nesclk = '1' and pulses = 7 ) then
					led(0) <= not nes_data;
					nextstate <= state1;	
				end if;				
				
		end case;	
	
	end process;

end Behavioral;
