/*
	Copyright 2012 bigbiff/Dees_Troy TeamWin
	This file is part of TWRP/TeamWin Recovery Project.

	TWRP is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	TWRP is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with TWRP.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _TWRPFUNCTIONS_HPP
#define _TWRPFUNCTIONS_HPP

#include <string>
#include <vector>

#include "twrpDigest/twrpDigest.hpp"

#ifndef BUILD_TWRPTAR_MAIN
#include "partitions.hpp"
#endif

using namespace std;

#define CACHE_LOGS_DIR "/cache/" // For devices with a dedicated cache partition
#define DATA_LOGS_DIR "/data/"	 // For devices that do not have a dedicated cache partition

typedef enum
{
	rb_current = 0,
	rb_system,
	rb_recovery,
	rb_poweroff,
	rb_bootloader,
	rb_download,
	rb_edl,
	rb_fastboot
} RebootCommand;

enum Archive_Type {
	UNCOMPRESSED = 0,
	COMPRESSED,
	ENCRYPTED,
	COMPRESSED_ENCRYPTED
};

// Partition class
class TWFunc
{
public:
	static string Get_Root_Path(const string& Path);                            // Trims any trailing folders or filenames from the path, also adds a leading / if not present
	static string Get_Path(const string& Path);                                 // Trims everything after the last / in the string
	static string Get_Filename(const string& Path);                             // Trims the path off of a filename

	static int Exec_Cmd(const string& cmd, string &result, bool combine_stderr);     //execute a command and return the result as a string by reference, set combined_stderror to add stderr
	static int Exec_Cmd(const string& cmd, bool Show_Errors = true);            //execute a command, displays an error to the GUI if Show_Errors is true, Show_Errors is true by default
	static int Wait_For_Child(pid_t pid, int *status, string Child_Name, bool Show_Errors = true); // Waits for pid to exit and checks exit status, displays an error to the GUI if Show_Errors is true which is the default
	static int Wait_For_Child_Timeout(pid_t pid, int *status, const string& Child_Name, int timeout); // Waits for a pid to exit until the timeout is hit. If timeout is hit, kill the chilld.
	static bool Path_Exists(string Path);                                       // Returns true if the path exists
	static Archive_Type Get_File_Type(string fn);                               // Determines file type, 0 for unknown, 1 for gzip, 2 for OAES encrypted
	static int Try_Decrypting_File(string fn, string password); // -1 for some error, 0 for failed to decrypt, 1 for decrypted, 3 for decrypted and found gzip format
	static unsigned long Get_File_Size(const string& Path);                     // Returns the size of a file
	static std::string Remove_Beginning_Slash(const std::string& path);         // Remove the beginning slash of a path
	static std::string Remove_Trailing_Slashes(const std::string& path, bool leaveLast = false); // Normalizes the path, e.g /data//media/ -> /data/media
	static void Strip_Quotes(char* &str);                                       // Remove leading & trailing double-quotes from a string
	static vector<string> split_string(const string &in, char del, bool skip_empty);
	static timespec timespec_diff(timespec& start, timespec& end);	            // Return a diff for 2 times
	static int32_t timespec_diff_ms(timespec& start, timespec& end);            // Returns diff in ms
	static bool Wait_For_File(const string& path, std::chrono::nanoseconds timeout); // Wait For File, True is success, False is timeout;
	static bool Wait_For_Battery(std::chrono::nanoseconds timeout);             // Wait For /sys/class/power_supply/battery or TW_CUSTOM_BATTERY_PATH, True is success, False is timeout;
	static bool File_Exists(const string& path);                                // File exists
	static void Screenshot(const string& path);                                 // Screenshot

#ifndef BUILD_TWRPTAR_MAIN
	static int Recursive_Mkdir(string Path);                                    // Recursively makes the entire path
	static void GUI_Operation_Text(string Read_Value, string Default_Text);     // Updates text for display in the GUI, e.g. Backing up %partition name%
	static void GUI_Operation_Text(string Read_Value, string Partition_Name, string Default_Text); // Same as above but includes partition name
	static void Update_Log_File(void);                                          // Writes the log to last_log
	static void Update_Intent_File(string Intent);                              // Updates intent file
	static int tw_reboot(RebootCommand command);                                // Prepares the device for rebooting
	static void check_and_run_script(const char* script_file, const char* display_name); // checks for the existence of a script, chmods it to 755, then runs it
	static int removeDir(const string path, bool removeParent); //recursively remove a directory
	static int copy_file(string src, string dst, int mode, bool mount_paths=true); //copy file from src to dst with mode permissions
	static unsigned int Get_D_Type_From_Stat(string Path);                      // Returns a dirent dt_type value using stat instead of dirent
	static int read_file(string fn, vector<string>& results); //read from file
	static int read_file(string fn, string& results); //read from file
	static int read_file(string fn, uint64_t& results); //read from file
	static bool write_to_file(const string& fn, const string& line);              //write single line to file with no newline
	static bool write_to_file(const string& fn, const std::vector<string> lines); // write vector of strings line by line with newlines
	static bool Try_Decrypting_Backup(string Restore_Path, string Password); // true for success, false for failed to decrypt
	static string System_Property_Get(string Prop_Name);                // Returns value of Prop_Name from reading /system/build.prop
	static string Partition_Property_Get(string Prop_Name, TWPartitionManager &PartitionManager, string Mount_Point, string prop_file_name);     // Returns value of Prop_Name from reading provided prop file
	static string Get_Current_Date(void);                               // Returns the current date in ccyy-m-dd--hh-nn-ss format
	static void Auto_Generate_Backup_Name();                            // Populates TW_BACKUP_NAME with a backup name based on current date and ro.build.display.id from /system/build.prop
	static void Fixup_Time_On_Boot(const string& time_paths = ""); // Fixes time on devices which need it (time_paths is a space separated list of paths to check for ats_* files)
	static std::vector<std::string> Split_String(const std::string& str, const std::string& delimiter, bool removeEmpty = true); // Splits string by delimiter
	static bool Create_Dir_Recursive(const std::string& path, mode_t mode = 0755, uid_t uid = -1, gid_t gid = -1);  // Create directory and it's parents, if they don't exist. mode, uid and gid are set to all _newly_ created folders. If whole path exists, do nothing.
	static int Set_Brightness(std::string brightness_value); // Well, you can read, it does what it says, passing return int from TWFunc::Write_File ;)
	static bool Toggle_MTP(bool enable);                                        // Disables MTP if enable is false and re-enables MTP if enable is true and it was enabled the last time it was toggled off
	static std::string to_string(unsigned long value); //convert ul to string
	static void SetPerformanceMode(bool mode); // support recovery.perf.mode
	static void Disable_Stock_Recovery_Replace(); // Disable stock ROMs from replacing TWRP with stock recovery
	static unsigned long long IOCTL_Get_Block_Size(const char* block_device);
	static void copy_kernel_log(string curr_storage); // Copy Kernel Log to Current Storage (PSTORE/KMSG)
	static void copy_logcat(string curr_storage); // Copy Logcat to Current Storage
	static bool isNumber(string strtocheck); // return true if number, false if not a number
	static int stream_adb_backup(string &Restore_Name); // Tell ADB Backup to Stream to TWRP from GUI selection
	static std::string get_log_dir(); // return recovery log storage directory
	static void check_selinux_support(); // print whether selinux support is enabled to console
	static bool Is_TWRP_App_In_System(); // Check if the TWRP app is installed in the system partition
	static void checkforapp();
	static int Property_Override(string Prop_Name, string Prop_Value); // Override properties (including ro. properties)
	static int Delete_Property(string Prop_Name); // Delete properties (non-persistent properties only)
	static void List_Mounts(); // List current mounts by the kernel
	static void Clear_Bootloader_Message(); // Removes the bootloader message from misc for next boot
	static string Check_For_TwrpFolder(); // Gets user defined path on storage where backups should be stored
	static bool Check_Xml_Format(const std::string filename); // Return whether a xml is in plain xml or ABX format
	static bool Find_Fstab(string &fstab);
	static bool Get_Service_From(TWPartition *Partition, std::string Service, std::string &Ret);
	static std::string Get_Version_From_Service(std::string name);

	static bool abx_to_xml(const std::string path, std::string &result); // could we convert abx to xml (if so, return the full path to the converted file)
private:
	static void Copy_Log(string Source, string Destination);

};

extern int Log_Offset;
#else
};
#endif // ndef BUILD_TWRPTAR_MAIN

#endif // _TWRPFUNCTIONS_HPP
