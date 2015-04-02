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
-- ( EDK: VGA 20x15 Resolution, Main VHDL )
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

entity vga is
   port (
      clk: in std_logic;
		--reset: in std_logic;
		hsync, vsync: out  std_logic;
      rgb: out std_logic_vector(2 downto 0);
      
		we: in std_logic;
		--sel: in std_logic_vector(1 downto 0);
		add_bus1: in std_logic_vector(8 downto 0);
		add_bus2: in std_logic_vector(8 downto 0);
		add_bus3: in std_logic_vector(10 downto 0);
		
		data_bus1: in std_logic_vector(4 downto 0);
		data_bus2: in std_logic_vector(4 downto 0);
		data_bus3: in std_logic_vector(3 downto 0)		
   );
end vga;

architecture Behavioral of vga is

	signal rgb_reg: std_logic_vector(2 downto 0);
   signal video_on: std_logic;
   signal pixel_x, pixel_y : std_logic_vector(9 downto 0);
			
	-- Foreground Tile-map : 20x15
	Type tile_map is array(0 to 299) of std_logic_vector(4 downto 0);
	signal fore_map : tile_map;     --(RAM 1)
	
	-- Background Tile-map : 20x15
	signal back_map : tile_map;     --(RAM 2)
	
	-- Sprites-map of 64x32
	Type sprites_map is array(0 to 2047) of std_logic_vector(3 downto 0);
	signal sprites : sprites_map;  -- (RAM 3)
		
		
   component vgatimehelper
     port (
       clk, reset       : in std_logic;
       hsync, vsync     : out std_logic;
       video_on, p_tick : out std_logic;
       pixel_x, pixel_y : out std_logic_vector(9 downto 0));
   end component;
	
	signal index, index1, index2: integer;
	signal reset: std_logic := '0';
	
	--signal data_buf1: std_logic_vector(4 downto 0);
	--signal data_buf2: std_logic_vector(4 downto 0);
	--signal data_buf3: std_logic_vector(3 downto 0);
	
begin

  -- instantiate VGA sync circuit
   vga_unit: vgatimehelper
      port map(clk=>clk, reset=>reset, hsync=>hsync,
               vsync=>vsync, video_on=>video_on,
               p_tick=>open, pixel_x=>pixel_x, pixel_y=>pixel_y);


	-- Creating, reading, and writing RAM 1:
	ram1: process
	begin
		if rising_edge(clk) then
			-- write:
			if( we = '1' ) then
				--fore_map(conv_integer(add_bus1)) <= data_buf1;
				fore_map(conv_integer(add_bus1)) <= data_bus1;
			end if;		
		end if;	
	end process;
	
	-- Creating, reading, and writing RAM 2:
	ram2: process
	begin
		if rising_edge(clk) then
			-- write:
			if( we = '1' ) then
				--back_map(conv_integer(add_bus2)) <= data_buf2;
				back_map(conv_integer(add_bus2)) <= data_bus2;
			end if;		
		end if;	
	end process;
	
	-- Creating, reading, and writing RAM 3:
	ram3: process
	begin
		if rising_edge(clk) then
			-- write:
			if( we = '1' ) then
				--sprites(conv_integer(add_bus3)) <= data_buf3;
				sprites(conv_integer(add_bus3)) <= data_bus3;
			end if;		
		end if;	
	end process;
		
	--data_buf1 <= data_bus1;
	--data_buf2 <= data_bus2;
	--data_buf3 <= data_bus3;
	
	-- Draw on the LCD screen:
	-- Conversion from 640x480 scale to 20x15 scale:
	-- from screen to foreground tile-map:
	index  <=  conv_integer(pixel_y(9 downto 5))*20 +  conv_integer(pixel_x(9 downto 5));
	
	index1 <= (conv_integer(pixel_y(4 downto 2)))*8 + (conv_integer(pixel_x(4 downto 2))) +
	          (conv_integer(fore_map(index))*64);
	
	index2 <= (conv_integer(pixel_y(4 downto 2)))*8 + (conv_integer(pixel_x(4 downto 2))) +
	          (conv_integer(back_map(index))*64);
				 
--	rgb_reg1 <= sprites(index1);
--	rgb_reg2 <= sprites(index2);
	rgb_reg <= sprites(index1)(2 downto 0) when sprites(index1)(3) = '0' else sprites(index2)(2 downto 0);
		
	rgb <= rgb_reg when video_on='1' else "000";
		
end Behavioral;
