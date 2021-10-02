#include "severity_t.h"
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/program_options/errors.hpp>
#include <stdexcept>


using namespace LotosPP::Log;
using namespace std;


severity_level severity_t::to_severity(const std::string& name)
{
	if (severityBimap::right_const_iterator it=severityMap.right.find(boost::algorithm::to_upper_copy(name)); it!=severityMap.right.end()) {
		return it->second;
		}
	using poverr=boost::program_options::validation_error;
	throw poverr(poverr::invalid_option_value);
}

const std::string severity_t::to_string() const
{
	if (severityBimap::left_const_iterator it=severityMap.left.find(level); it!=severityMap.left.end()) {
		return it->second;
		}
	throw runtime_error("can't convert severity to string");
}

const std::string severity_t::to_string(const severity_level lvl)
{
	if (severityBimap::left_const_iterator it=severityMap.left.find(lvl); it!=severityMap.left.end()) {
		return it->second;
		}
	throw runtime_error("can't convert severity lvl to string");
}

std::ostream& LotosPP::Log::operator<<(std::ostream& os, const severity_t& sev)
{
	os << sev.to_string();
	return os;
}

std::istream& LotosPP::Log::operator>>(std::istream& is, severity_t& sev)
{
	string name;
	is >> name;
	sev.level=severity_t::to_severity(name);
	return is;
}

namespace LotosPP::Log { // why are these required ?
template<typename CharT, typename TraitsT>
std::basic_ostream<CharT, TraitsT>& operator<<(std::basic_ostream<CharT, TraitsT>& strm, severity_level lvl)
{
	try {
		strm << severity_t::to_string(lvl);
		}
	catch (runtime_error) {
		strm << static_cast<int>(lvl);
		}
	return strm;
}
} // namespaces
