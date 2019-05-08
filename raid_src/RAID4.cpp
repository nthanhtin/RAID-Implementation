#include <bits/stdc++.h>

using namespace std;

class Metadata
{
private:
    map<string, pair<long long, long long>> m;
    ofstream meta;
    string meta_file_name;

public:
    Metadata(string meta_file);
    ~Metadata()
    {
        meta.close();
    }
    bool search_file(string file_name);
    bool delete_file(string file_name);
    void print_meta(void);
    void insert(string file_name, long long file_length, long long level_count);
    void list_files(void);
    void write_meta_file(void);
    pair<long long, long long> get(string file_name);
};

Metadata::Metadata(string meta_file)
{
    meta_file_name = meta_file;
    ifstream meta_read(meta_file);
    char temp;
    string raw[3];
    long long i = 0;
    while (meta_read.get(temp))
    {
        if (temp == ':')
        {
            i++;
        }
        else if (temp == '\n')
        {
            i = 0;
            m.insert({raw[0], {stoi(raw[1]), stoi(raw[2])}});
            raw[0].clear();
            raw[1].clear();
            raw[2].clear();
        }
        else
        {
            raw[i] += temp;
        }
    }
    meta_read.close();
    meta.open(meta_file, ofstream::app);
}

bool Metadata::search_file(string file_name)
{
    if (m.find(file_name) == m.end())
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool Metadata::delete_file(string file_name)
{
    if (m.find(file_name) == m.end())
        return false;
    m.erase(m.find(file_name));
    write_meta_file();
    return true;
}

void Metadata::list_files(void)
{
    cout << "____________________________________________________________________" << endl;
    for (auto i : m)
    {
        cout << i.first;
        for (long long j = 0; j < 50 - (i.first).size(); j++)
            cout << " ";
        cout << i.second.first << " bytes" << endl;
    }
    cout << "____________________________________________________________________" << endl;
}

void Metadata::print_meta(void)
{
    for (auto i : m)
    {
        cout << i.first << " " << i.second.first << " " << i.second.second << endl;
    }
}

void Metadata::insert(string file_name, long long file_length, long long level_count)
{
    meta << file_name.c_str() << ":" << to_string(file_length) << ":" << to_string(level_count) << endl;
    m.insert({file_name, {file_length, level_count}});
}

void Metadata::write_meta_file(void)
{
    meta.close();
    remove(meta_file_name.c_str());
    meta.open(meta_file_name, ofstream::app);
    for (auto i : m)
    {
        meta << i.first << ":" << i.second.first << ":" << i.second.second << endl;
    }
}

pair<long long, long long> Metadata::get(string file_name)
{
    return m[file_name];
}

void access_disk_write(vector<ofstream> &disk_write)
{
    disk_write[0].open("disk_1", ofstream::app);
    disk_write[1].open("disk_2", ofstream::app);
    disk_write[2].open("disk_3", ofstream::app);
    disk_write[3].open("disk_4", ofstream::app);
    disk_write[4].open("disk_5", ofstream::app);
}

void access_disk_read(vector<ifstream> &disk_read)
{
    disk_read[0].open("disk_1");
    disk_read[1].open("disk_2");
    disk_read[2].open("disk_3");
    disk_read[3].open("disk_4");
    disk_read[4].open("disk_5");
}

void close_disk_read(vector<ifstream> &disk_read)
{
    for (long long i = 0; i < 5; i++)
    {
        disk_read[i].close();
    }
}

void close_disk_write(vector<ofstream> &disk_write)
{
    for (long long i = 0; i < 5; i++)
    {
        disk_write[i].close();
    }
}

bool check_drives(void)
{
    long long drives_failed = 0;
    string file_name = "disk_";

    for (long long i = 1; i <= 5; i++)
        if (!ifstream(file_name + (char)(i + '0')))
            drives_failed++;
    if (drives_failed == 0)
        return true;
    else
        return false;
}

int main()
{
    Metadata metadata("meta");

    vector<ofstream> disk_write(5);
    vector<ifstream> disk_read(5);

    access_disk_write(disk_write);
    close_disk_write(disk_write);

    string command;
    while (true)
    {
        cout << "> ";
        getline(cin, command);

        if (command.find("exit") == 0)
        {
            break;
        }
        else if (command.find("write") == 0)
        {
            if (!check_drives())
            {
                cout << "Some of the drives have failed, try to recover" << endl;
                continue;
            }

            string file_path = command.substr(6, command.size() - 6);
            if (metadata.search_file(file_path))
            {
                cout << "File already exists" << endl;
                continue;
            }
            if (!ifstream(file_path))
            {
                cout << "Invalid file/location" << endl;
                continue;
            }
            ifstream file(file_path);

            access_disk_write(disk_write);
            file.seekg(0, file.end);
            long long file_length = file.tellg();
            long long level_count = disk_write[0].tellp();
            long long file_size = file_length;
            file.seekg(0, file.beg);

            metadata.insert(file_path, file_length, level_count);

            char buffer[4];
            long long factor = 0;

            system("setterm -cursor off");

            cout << "000%";
            while (true)
            {
                file.read(buffer, 4);
                char parity = buffer[0] ^ buffer[1] ^ buffer[2] ^ buffer[3];

                long long parity_location = 4 - factor;
                long long buffer_location = 0;
                factor = (factor + 1) % 5;
                for (long long i = 0; i < 5; i++)
                {
                    if (i == parity_location)
                    {
                        disk_write[i].put(parity);
                    }
                    else
                    {
                        disk_write[i].put(buffer[buffer_location++]);
                    }
                }
                file_length -= 4;

                long long percentage = ((file_size - file_length) * 100) / file_size;
                if (percentage > 100)
                    percentage = 100;
                cout << (char)8 << (char)8 << (char)8 << (char)8;
                cout << setfill('0') << setw(3) << percentage;
                cout << '%';

                if (file_length <= 0)
                    break;
            }
            file.close();

            close_disk_write(disk_write);
            cout << " - done" << endl;

            system("setterm -cursor on");
        }
        else if (command.find("read") == 0)
        {
            if (!check_drives())
            {
                cout << "Some of the drives have failed, try to recover" << endl;
                continue;
            }
            string file_path = command.substr(5, command.size() - 5);

            if (!metadata.search_file(file_path))
            {
                cout << "File doesn't exists" << endl;
                continue;
            }
            access_disk_read(disk_read);
            ofstream file("raid_temp_file");

            auto result = metadata.get(file_path);
            long long length = result.first;
            long long total_length = length;
            long long level_count = result.second;

            for (long long i = 0; i < 5; i++)
                disk_read[i].seekg(level_count, disk_read[i].beg);

            long long factor = 0;
            char temp;
            bool flag = true;

            system("setterm -cursor off");

            cout << "000%";
            while (flag)
            {
                long long parity_location = 4 - factor;
                long long buffer_location = 0;
                factor = (factor + 1) % 5;
                for (long long i = 0; i < 5; i++)
                {
                    if (i != parity_location)
                    {
                        disk_read[i].get(temp);
                        file.put(temp);
                        length--;
                        if (length == 0)
                        {
                            flag = false;
                            break;
                        }
                    }
                    else
                    {
                        disk_read[i].get(temp);
                    }
                }

                long long percentage = ((total_length - length) * 100) / total_length;
                if (percentage > 100)
                    percentage = 100;
                cout << (char)8 << (char)8 << (char)8 << (char)8;
                cout << setfill('0') << setw(3) << percentage;
                cout << '%';
            }
            file.close();
            close_disk_read(disk_read);
            cout << " - done" << endl;

            system("setterm -cursor on");
        }
        else if (command.find("recover") == 0)
        {
            long long drives_failed = 0, x = 0;
            string failed_drive;
            string file_name = "disk_";

            for (long long i = 1; i <= 5; i++)
            {
                if (!ifstream(file_name + (char)(i + '0')))
                {
                    drives_failed++;
                    failed_drive = file_name + (char)(i + '0');
                    x = i;
                }
            }

            if (drives_failed == 0)
            {
                cout << "All drives OK!" << endl;
            }
            else if (drives_failed == 1)
            {
                cout << failed_drive << " failed!" << endl;
                vector<ifstream> alive_drive(4);
                ofstream dead_drive(failed_drive);
                long long counter = 0;
                for (long long i = 1; i <= 5; i++)
                {
                    if (i != x)
                    {
                        alive_drive[counter++].open(file_name + (char)(i + '0'));
                    }
                }
                char buffer[4];
                while (alive_drive[0].get(buffer[0]) && alive_drive[1].get(buffer[1]) && alive_drive[2].get(buffer[2]) && alive_drive[3].get(buffer[3]))
                {
                    char recovery_byte = buffer[0] ^ buffer[1] ^ buffer[2] ^ buffer[3];
                    dead_drive.put(recovery_byte);
                }
                cout << failed_drive << " recovered" << endl;
                dead_drive.close();
                alive_drive[0].close();
                alive_drive[1].close();
                alive_drive[2].close();
                alive_drive[3].close();
            }
            else
            {
                cout << drives_failed << " drives failed, can't recover" << endl;
            }
        }
        else if (command.find("ls") == 0)
        {
            metadata.list_files();
        }
        else if (command.find("delete") == 0)
        {
            if (!check_drives())
            {
                cout << "Some of the drives have failed, try to recover" << endl;
                continue;
            }
            string file_path = command.substr(7, command.size() - 7);
            if (metadata.delete_file(file_path))
                cout << "File deleted" << endl;
            else
                cout << "File not found!" << endl;
        }
        else
        {
            cout << "Wrong command!" << endl;
        }
    }
    return 0;
}
