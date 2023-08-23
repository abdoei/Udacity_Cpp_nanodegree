#include "../src/util.hpp"
#include <gtest/gtest.h>
#include "../src/SysInfo.hpp"
#include "../src/ProcessParser.hpp"
#include "../src/Process.hpp"

namespace {
    TEST(constantsTest, convertToTime) {
        EXPECT_THROW(Util::convertToTime(-1), std::runtime_error);
        EXPECT_EQ(Util::convertToTime(    0), "0:0:0");
        EXPECT_EQ(Util::convertToTime(    1), "0:0:1");
        EXPECT_EQ(Util::convertToTime(   60), "0:1:0");
        EXPECT_EQ(Util::convertToTime( 3600), "1:0:0");
        EXPECT_EQ(Util::convertToTime( 3661), "1:1:1");
        EXPECT_EQ(Util::convertToTime(86400), "24:0:0");
    }

    TEST(constantsTest, getProgressBar) {
        EXPECT_EQ(Util::getProgressBar( "-1"), "0% |                                                  -1 /100%");
        EXPECT_EQ(Util::getProgressBar(  "0"), "0% |                                                  0 /100%");
        EXPECT_EQ(Util::getProgressBar( "50"), "0% ||||||||||||||||||||||||||                         50 /100%");
        EXPECT_EQ(Util::getProgressBar("100"), "0% |||||||||||||||||||||||||||||||||||||||||||||||||| 100 /100%");
        EXPECT_EQ(Util::getProgressBar("101"), "0% |||||||||||||||||||||||||||||||||||||||||||||||||| 101 /100%");
    }

    TEST(constantsTest, getStream) {
        EXPECT_THROW(Util::getStream("nonexistentfile"), std::runtime_error);
        EXPECT_NO_THROW(Util::getStream("Makefile"));
    }

    TEST(processParserTest, getCmd) {
        EXPECT_THROW(ProcessParser::getCmd("-1"), std::runtime_error);
        EXPECT_THROW(ProcessParser::getCmd("0"), std::runtime_error);
        EXPECT_THROW(ProcessParser::getCmd("nonexistentprocess"), std::runtime_error);
        EXPECT_STREQ(ProcessParser::getCmd("1").c_str(), "/sbin/init");
    }

    TEST(processParserTest, getPidList) {
        EXPECT_NO_THROW(ProcessParser::getPidList());
        EXPECT_EQ(ProcessParser::getPidList().size() > 0, true);
    }

    TEST(processParserTest, getVmSize) {
        EXPECT_THROW(ProcessParser::getVmSize("-1"), std::runtime_error);
        EXPECT_THROW(ProcessParser::getVmSize("0"), std::runtime_error);
        EXPECT_THROW(ProcessParser::getVmSize("nonexistentprocess"), std::runtime_error);
        EXPECT_EQ(stof(ProcessParser::getVmSize("1")) > 0.0, true);
    }

    TEST(processParserTest, getCpuPercent) {
        EXPECT_THROW(ProcessParser::getCpuPercent("-1"), std::runtime_error);
        EXPECT_THROW(ProcessParser::getCpuPercent("0"), std::runtime_error);
        EXPECT_THROW(ProcessParser::getCpuPercent("nonexistentprocess"), std::runtime_error);
        EXPECT_EQ(stof(ProcessParser::getCpuPercent("1")) > 0.0, true);
    }

    TEST(ProcessParserTest, getProcUser) {
        EXPECT_THROW(ProcessParser::getProcUser("-1"), std::runtime_error);
        EXPECT_THROW(ProcessParser::getProcUser("0"), std::runtime_error);
        EXPECT_THROW(ProcessParser::getProcUser("nonexistentprocess"), std::runtime_error);
        EXPECT_STREQ(ProcessParser::getProcUser("1").c_str(), "root");
    }

    TEST(ProcessParserTest, getNumberOfCores) {
        EXPECT_NO_THROW(ProcessParser::getNumberOfCores());
        EXPECT_EQ(ProcessParser::getNumberOfCores() >= 1, true);
    }

    TEST(ProcessParserTest, getSysCpuPercent) {
        EXPECT_NO_THROW(ProcessParser::getSysCpuPercent("0"));
        EXPECT_EQ(stoi(ProcessParser::getSysCpuPercent()[1]) > 1, true);
    }

    TEST(ProcessParserTest, getSysRamPercent) {
        EXPECT_NO_THROW(ProcessParser::getSysRamPercent());
        EXPECT_EQ(ProcessParser::getSysRamPercent() > 0.0, true);
    }

    TEST(ProcessParserTest, getSysKernelVersion) {
        EXPECT_NO_THROW(ProcessParser::getSysKernelVersion());
        EXPECT_EQ(ProcessParser::getSysKernelVersion().size() > 0, true);
    }

    TEST(ProcessParserTest, getOsName) {
        EXPECT_NO_THROW(ProcessParser::getOsName());
        EXPECT_EQ(ProcessParser::getOsName().size() > 0, true);
    }

    TEST(ProcessParserTest, getTotalThreads) {
        EXPECT_NO_THROW(ProcessParser::getTotalThreads());
        EXPECT_EQ(ProcessParser::getTotalThreads() > 0, true);
    }

    TEST(ProcessParserTest, getTotalNumberOfProcesses) {
        EXPECT_NO_THROW(ProcessParser::getTotalNumberOfProcesses());
        EXPECT_EQ(ProcessParser::getTotalNumberOfProcesses() > 0, true);
    }

    TEST(ProcessParserTest, getNumberOfRunningProcesses) {
        EXPECT_NO_THROW(ProcessParser::getNumberOfRunningProcesses());
        EXPECT_EQ(ProcessParser::getNumberOfRunningProcesses() > 0, true);
    }

    TEST(SysInfoTest, getters) {
        SysInfo system;
        EXPECT_NO_THROW(system.getUpTime());
        EXPECT_EQ(system.getUpTime() > 0, true);
        EXPECT_NO_THROW(system.getOsName());
        EXPECT_EQ(system.getOsName().size() > 0, true);
        EXPECT_NO_THROW(system.getKernelVersion());
        EXPECT_EQ(system.getKernelVersion().size() > 0, true);
        EXPECT_NO_THROW(system.getCpuPercent());
        EXPECT_EQ(stof(system.getCpuPercent()) > 0.0, true);
        EXPECT_NO_THROW(system.getTotalProc());
        EXPECT_EQ(stof(system.getTotalProc()) > 0, true);
        EXPECT_NO_THROW(system.getRunningProc());
        EXPECT_EQ(stof(system.getRunningProc()) > 0, true);
        EXPECT_NO_THROW(system.getThreads());
        EXPECT_EQ(stof(system.getThreads()) > 0, true);
        EXPECT_NO_THROW(system.getMemPercent());
        EXPECT_EQ(stof(system.getMemPercent()) > 0.0, true);
        EXPECT_NO_THROW(system.getCoresStats());
        EXPECT_EQ(system.getCoresStats().size() > 0, true);
        EXPECT_NO_THROW(system.getOtherCores(ProcessParser::getNumberOfCores()));
    }

    TEST(SysInfoTest, setters) {
        SysInfo system;
        EXPECT_NO_THROW(system.setLastCpuMeasures());
        EXPECT_NO_THROW(system.setAttributes());
        EXPECT_NO_THROW(system.setCpuCoresStats());
    }

    TEST(ProcessTest, getters) {
        Process process("1");
        EXPECT_NO_THROW(process.getProcess());
        EXPECT_EQ(process.getProcess().size() > 0, true);
        EXPECT_NO_THROW(process.getPid());
        EXPECT_EQ(stoi(process.getPid()), 1);
        EXPECT_NO_THROW(process.getUser());
        EXPECT_EQ(process.getUser(), "root");
        EXPECT_NO_THROW(process.getCmd());
        EXPECT_EQ(process.getCmd().size() > 0, true);
        EXPECT_NO_THROW(process.getCpu());
        EXPECT_EQ(process.getCpu() > 0.0, true);
        EXPECT_NO_THROW(process.getMem());
        EXPECT_EQ(process.getMem() > 0.0, true);
        EXPECT_NO_THROW(process.getUpTime());
        EXPECT_EQ(process.getUpTime().size() > 0, true);
    }

    TEST(ProcessTest, formatCMDTest){
        Process process("1");
        EXPECT_NO_THROW(process.formatCMD("/path/to/a/very/long/command/that/should/be/truncated"));
        EXPECT_EQ(process.formatCMD("/home/abdoeid/miniconda3/x86_64-conda_cos7-linux-gnu/bin"), "/home/abdoeid/miniconda3/x86_64-con/bin");
    }

} // namespace
int main(int argc, char **argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}