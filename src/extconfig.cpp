/*
 * Sound card modem for Amateur Radio AX25.
 *
 * Copyright (C) Alejandro Santos, 2013, alejolp@gmail.com.
 *
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <iostream>
#include <fstream>

#include "extconfig.h"

namespace extmodem {

boost::shared_ptr<config> config::config_instance_;

config* config::Instance() {
	if (!config_instance_.get())
		config_instance_.reset(new config());
	return config_instance_.get();
}

config::config() : desc("Allowed options") {}

config::~config() {}

bool config::is_help() {
	if (vm.count("help")) {
		std::cout << desc << std::endl;
		return true;
	}
	return false;
}

void config::init(int argc, char** argv) {
	desc.add_options()
	    ("help,h", "produce help message")
	    ("debug,d", "produce debug messages")
	    ("config-file", po::value<std::string>(), "configuration file name")
	    ("kiss-tcp-port", po::value<int>(&kiss_tcp_port_)->default_value(6666), "set KISS TCP listening port")
	    ("agwpe-tcp-port", po::value<int>(&agwpe_tcp_port_)->default_value(8000), "set AGWPE emulator TCP listening port")
	    ("ptt-port", po::value<std::string>(&ptt_port_)->default_value("/dev/ttyS0"), "set serial port PTT name")
	    ("tx-delay", po::value<int>(&tx_delay_)->default_value(200), "set tx-delay in ms")
	    ("tx-tail", po::value<int>(&tx_tail_)->default_value(50), "set tx-tail in ms")
	;

	p_.add("config-file", -1);

	po::store(po::command_line_parser(argc, argv).options(desc).positional(p_).run(), vm);
	po::notify(vm);

	if (vm.count("config-file")) {
		std::ifstream input_file(vm["config-file"].as<std::string>().c_str());
		if (input_file) {
			po::store(po::parse_config_file(input_file, desc), vm);
			po::notify(vm);
		} else {
			throw std::exception(); // FIXME
		}
	}

	debug_ = (vm.count("debug") > 0);
}


} /* namespace extmodem */
