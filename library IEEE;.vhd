library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity controller is
    port (	clk			: in	std_logic;
		reset			: in	std_logic;

		sensor_l		: in	std_logic;
		sensor_m		: in	std_logic;
		sensor_r		: in	std_logic;

		mine_sensor		: in	std_logic;
		data_in			: out	std_logic_vector(7 downto 0);
		data_out		: in	std_logic_vector(7 downto 0);
		write_data		: out	std_logic;
		read_data		: out	std_logic;
		new_data		: in	std_logic;

		count_in		: in	std_logic_vector (19 downto 0);
		count_reset		: out	std_logic;

		motor_l_reset		: out	std_logic;
		motor_l_direction	: out	std_logic;

		motor_r_reset		: out	std_logic;
		motor_r_direction	: out	std_logic
	);
end entity controller;

architecture controller_arch of controller is
	type controller_state is (	reset_state,
					reset_state2,
					mine_reset,
					turnaround,
					turnaround_reset,
					pass_crossing,
					crossing_reset,
					crossing,
					turn_reset,
					turn_right,
					turn_left,
					backward,
					backward_right,
					backward_left,
					backwardreset,
					backward_reset,
					station,
					station_reset,
					gentleleft,
					sharpleft,
					gentleright,
					sharpright,
					forward);
	signal state, new_state:	controller_state;
	signal sensors: std_logic_vector(2 downto 0);
	signal mine_detect: std_logic;
	signal mine_count, mine_new_count: unsigned(19 downto 0);
	signal turn_count, turn_new_count: unsigned(2 downto 0);

	signal data: std_logic_vector(7 downto 0);

begin
	process(clk)
	begin
		if(rising_edge (clk)) then
			if(reset = '1') then
				state <= reset_state;
			else
				state <= new_state;
			end if;
		end if;
	end process;

	process(clk, mine_count, mine_sensor)
	begin
		if(rising_edge (clk)) then
			if(reset = '1') then
				mine_count <= (others => '0');
			elsif(mine_sensor = '1') then
				if(mine_count > "00000000100111000100") then
					mine_count <= mine_new_count;
				else
					mine_count <= (others => '0');
				end if;
			else
				mine_count <= mine_new_count;
			end if;
		end if;
	end process;
	
	process(mine_count)
	begin
		mine_new_count <= mine_count + 1;
	end process;

	process(mine_count)
	begin
		if mine_count > "00000000100111000100" then
			mine_detect <= '1';
		else
			mine_detect <= '0';
		end if;
	end process;

	process(turn_count, state)
	begin
		if state = crossing or state = backward then
			turn_new_count <= turn_count + 1;
		elsif state = turn_reset or state = turn_right or state = turn_left 
		or state = backward_reset then
			turn_count <= turn_new_count;
		else
			turn_count <= (others => '0');
			turn_new_count <= (others => '0');
		end if;
	end process;

	process(sensors, count_in, mine_detect, mine_count, turn_count, data_out)
	begin
		case state is
			when reset_state =>
				motor_l_reset <= '1';
				motor_r_reset <= '1';
				motor_l_direction <= 'X';
				motor_r_direction <= 'X';
				write_data <= '0';
				if sensors = "000" and data_out = "01101111" then
					new_state <= pass_crossing;
				elsif sensors = "000" then
					new_state <= crossing;
				elsif sensors = "001" then
					new_state <= gentleleft;
				elsif sensors = "010" then
					new_state <= forward;
				elsif sensors = "011" then
					new_state <= sharpleft;
				elsif sensors = "100" then
					new_state <= gentleright;
				elsif sensors = "101" then
					new_state <= forward;
				elsif sensors = "110" then
					new_state <= sharpright;
				elsif sensors = "111" then
					new_state <= station;
				else
					new_state <= reset_state;
				end if;
			when reset_state2 =>
				motor_l_reset <= '1';
				motor_r_reset <= '1';
				motor_l_direction <= 'X';
				motor_r_direction <= 'X';
				write_data <= '0';
				if data_out = data then
					new_state <= reset_state2;
				elsif data_out = "01101111" then
					new_state <= turnaround;
				elsif data_out="01110000" or data_out="01110001" then
					new_state <= backward;
				else
					new_state <= reset_state2;
				end if;
			when mine_reset =>
				motor_l_reset <= '1';
				motor_r_reset <= '1';
				motor_l_direction <= 'X';
				motor_r_direction <= 'X';
				
					write_data <= '1';
					data_in <= "01101101";
					data <= data_out;
					new_state <= reset_state2;
			when turnaround =>
				motor_l_reset <= '0';
				motor_r_reset <= '0';
				motor_l_direction <= '1';
				motor_r_direction <= '1';
				write_data <= '0';
				if count_in = "11110100001001000000" then
					new_state <= turnaround_reset;
				else
					new_state <= turnaround;
				end if;
			when turnaround_reset =>
				motor_l_reset <= '1';
				motor_r_reset <= '1';
				motor_l_direction <= 'X';
				motor_r_direction <= 'X';
				if sensors = "110" then
					new_state <= sharpright;
				else
					new_state <= turnaround;
				end if;
			when pass_crossing =>
				motor_l_reset <= '0';
				motor_r_reset <= '0';
				motor_l_direction <= '1';
				motor_r_direction <= '0';
				write_data <= '0';
				if count_in = "11110100001001000000" then
					new_state <= crossing_reset;
				else
					new_state <= pass_crossing;
				end if;
			when crossing_reset =>
				motor_l_reset <= '1';
				motor_r_reset <= '1';
				motor_l_direction <= 'X';
				motor_r_direction <= 'X';
				if sensor_l='1' or sensor_m='1' or sensor_r='1' then
					write_data <= '1';
					data_in <= "01100111";
					new_state <= reset_state;
				else
					new_state <= pass_crossing;
				end if;
			when crossing =>
				motor_l_reset <= '0';
				motor_r_reset <= '0';
				motor_l_direction <= '1';
				motor_r_direction <= '0';
				write_data <= '0';
				if count_in = "11110100001001000000" then
					new_state <= turn_reset;
				else
					new_state <= crossing;
				end if;
			when turn_reset =>
				motor_l_reset <= '1';
				motor_r_reset <= '1';
				motor_l_direction <= 'X';
				motor_r_direction <= 'X';
				if data_out = "01110010" then
					new_state <= crossing;
					if turn_count = "101" then
						new_state <= turn_right;
						if sensors = "110" then
							new_state <= sharpright;
							write_data <= '1';
							data_in <= "01100111";
						else
							new_state <= turn_right;
							write_data <= '0';
						end if;
					else
						new_state <= crossing;
					end if;
				end if;
				if data_out = "01101100" then
					new_state <= crossing;
					if turn_count = "101" then
						new_state <= turn_left;
						if sensors = "011" then
							new_state <= sharpleft;
							write_data <= '1';
							data_in <= "01100111";
						else
							new_state <= turn_left;
							write_data <= '0';
						end if;
					else
						new_state <= crossing;
					end if;
				end if;
				if data_out = "01100111" then
					new_state <= crossing;
					if turn_count = "101" then
						write_data <= '1';
						data_in <= "11100110";
						if sensors = "000" then
							new_state <= crossing;
						elsif sensors = "001" then
							new_state <= gentleleft;
						elsif sensors = "010" then
							new_state <= forward;
						elsif sensors = "011" then
							new_state <= sharpleft;
						elsif sensors = "100" then
							new_state <= gentleright;
						elsif sensors = "101" then
							new_state <= forward;
						elsif sensors = "110" then
							new_state <= sharpright;
						elsif sensors = "111" then
							new_state <= forward;
						else
							new_state <= turn_reset;
						end if;
					else
						new_state <= crossing;
						write_data <= '0';
					end if;
				end if;
			when turn_right =>
				motor_l_reset <= '0';
				motor_r_reset <= '0';
				motor_l_direction <= '1';
				motor_r_direction <= '1';
				write_data <= '0';
				if count_in = "11110100001001000000" then
					new_state <= turn_reset;
				else
					new_state <= turn_right;
				end if;
			when turn_left =>
				motor_l_reset <= '0';
				motor_r_reset <= '0';
				motor_l_direction <= '0';
				motor_r_direction <= '0';
				write_data <= '0';
				if count_in = "11110100001001000000" then
					new_state <= turn_reset;
				else
					new_state <= turn_left;
				end if;
			when backward =>
				motor_l_reset <= '0';
				motor_r_reset <= '0';
				motor_l_direction <= '0';
				motor_r_direction <= '1';
				write_data <= '0';
				if count_in = "11110100001001000000" then
					new_state <= backward_reset;
				else
					new_state <= backward;
				end if;
			when backward_right =>
				motor_l_reset <= '0';
				motor_r_reset <= '0';
				motor_l_direction <= '1';
				motor_r_direction <= '1';
				write_data <= '0';
				if count_in = "11110100001001000000" then
					new_state <= backwardreset;
				else
					new_state <= backward_right;
				end if;
			when backward_left =>
				motor_l_reset <= '0';
				motor_r_reset <= '0';
				motor_l_direction <= '0';
				motor_r_direction <= '0';
				write_data <= '0';
				if count_in = "11110100001001000000" then
					new_state <= backwardreset;
				else
					new_state <= backward_left;
				end if;
			when backwardreset =>
				motor_l_reset <= '1';
				motor_r_reset <= '1';
				motor_l_direction <= 'X';
				motor_r_direction <= 'X';
				if data_out = "01110000" then
					if sensors = "110" then
						write_data <= '1';
						data_in <= "01100111";
						new_state <= sharpright;
					else
						new_state <= backward_right;
					end if;
				elsif data_out = "01110001" then
					if sensors = "011" then
						write_data <= '1';
						data_in <= "01100111";
						new_state <= sharpleft;
					else
						new_state <= backward_left;
					end if;
				else 
					new_state <= backward_reset;
				end if;
			when backward_reset =>
				motor_l_reset <= '1';
				motor_r_reset <= '1';
				motor_l_direction <= 'X';
				motor_r_direction <= 'X';
				if turn_count = "101" and data_out = "01110000" then
					new_state <= backward_right;
				elsif turn_count = "101" and data_out="01110001" then
					new_state <= backward_left;
				else
					new_state <= backward;
				end if;
			when station =>
				motor_l_reset <= '1';
				motor_r_reset <= '1';
				write_data <= '1';
				data_in <= "01110100";
				new_state <= station_reset;
			when station_reset =>
				motor_l_reset <= '1';
				motor_r_reset <= '1';
				write_data <= '0';
			when gentleleft =>
				motor_l_reset <= '1';
				motor_r_reset <= '0';
				motor_l_direction <= 'X';
				motor_r_direction <= '0';
				write_data <= '0';
				if count_in = "11110100001001000000" then
					if mine_detect = '1' then
						new_state <= mine_reset;
					else
						new_state <= reset_state;
					end if;
				else
					new_state <= gentleleft;
				end if;
			when sharpleft =>
				motor_l_reset <= '0';
				motor_r_reset <= '0';
				motor_l_direction <= '0';
				motor_r_direction <= '0';
				write_data <= '0';
				if count_in = "11110100001001000000" then
					if mine_detect = '1' then
						new_state <= mine_reset;
					else
						new_state <= reset_state;
					end if;
				else
					new_state <= sharpleft;
				end if;
			when gentleright =>
				motor_l_reset <= '0';
				motor_r_reset <= '1';
				motor_l_direction <= '1';
				motor_r_direction <= 'X';
				write_data <= '0';
				if count_in = "11110100001001000000" then
					if mine_detect = '1' then
						new_state <= mine_reset;
					else
						new_state <= reset_state;
					end if;
				else
					new_state <= gentleright;
				end if;
			when sharpright =>
				motor_l_reset <= '0';
				motor_r_reset <= '0';
				motor_l_direction <= '1';
				motor_r_direction <= '1';
				write_data <= '0';
				if count_in = "11110100001001000000" then
					if mine_detect = '1' then
						new_state <= mine_reset;
					else
						new_state <= reset_state;
					end if;
				else
					new_state <= sharpright;
				end if;
			when forward =>
				motor_l_reset <= '0';
				motor_r_reset <= '0';
				motor_l_direction <= '1';
				motor_r_direction <= '0';
				write_data <= '0';
				if count_in = "11110100001001000000" then
					if mine_detect = '1' then
						new_state <= mine_reset;
					else
						new_state <= reset_state;
					end if;
				else
					new_state <= forward;
				end if;
		end case;
	end process;
count_reset <= '1' when state = reset_state or state = mine_reset or
state = turn_reset or state = turnaround_reset or state = reset_state2 or 
state = crossing_reset or state = backward_reset or state = backwardreset or 
state = station_reset
		else '0';

sensors(0) <= sensor_r;
sensors(1) <= sensor_m;
sensors(2) <= sensor_l;

end architecture controller_arch;