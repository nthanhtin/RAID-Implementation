#include <bits/stdc++.h>
#include <string>
#include "extractionraid1.h"
using namespace std;

void extract(string outfilepath)
{

	int parity_index = 0, n = 5;
    	string filepath=outfilepath;
	string source_filepath1="/home/nthanhtin/220/raid/raid_files/disk1";
	string source_filepath2="/home/nthanhtin/220/raid/raid_files/disk2";
	string source_filepath3="/home/nthanhtin/220/raid/raid_files/disk3";
	string source_filepath4="/home/nthanhtin/220/raid/raid_files/disk4";
	string source_filepath5="/home/nthanhtin/220/raid/raid_files/disk5";

	string source_filepath1c = "/home/nthanhtin/220/raid/raid_files/disk1c";
	string source_filepath2c = "/home/nthanhtin/220/raid/raid_files/disk2c";
	string source_filepath3c = "/home/nthanhtin/220/raid/raid_files/disk3c";
	string source_filepath4c = "/home/nthanhtin/220/raid/raid_files/disk4c";
	string source_filepath5c = "/home/nthanhtin/220/raid/raid_files/disk5c";

	FILE *fp = fopen(filepath.c_str(), "wb");
	FILE *f1 = fopen(source_filepath1.c_str(), "rb");
	FILE *f2 = fopen(source_filepath2.c_str(), "rb");
	FILE *f3 = fopen(source_filepath3.c_str(), "rb");
	FILE *f4 = fopen(source_filepath4.c_str(), "rb");
	FILE *f5 = fopen(source_filepath5.c_str(), "rb");
	if (f1 == NULL || f2 == NULL || f3 == NULL || f4 == NULL || f5 == NULL)
	{
		f1 = fopen(source_filepath1c.c_str(), "rb");
		f2 = fopen(source_filepath2c.c_str(), "rb");
		f3 = fopen(source_filepath3c.c_str(), "rb");
		f4 = fopen(source_filepath4c.c_str(), "rb");
		f5 = fopen(source_filepath5c.c_str(), "rb");
	}

	cout << "starting extraction..." << endl;
	while (1)
	{
		unsigned char buff[4] = {0}, temp;

		int j = 0, flag = 0;
		for (int i = 0; i < n && flag == 0; i++)
		{

			switch (i)
			{
			case 0:
				if (i == parity_index)
				{
					fread(&temp, 1, 1, f1);
				}
				else
				{
					int nread = fread(buff + j, 1, 1, f1);
					j++;
					if (nread <= 0)
						flag = 1;
				}
				break;
			case 1:
				if (i == parity_index)
				{
					fread(&temp, 1, 1, f2);
				}
				else
				{
					int nread = fread(buff + j, 1, 1, f2);
					j++;
					if (nread <= 0)
						flag = 1;
				}
				break;
			case 2:
				if (i == parity_index)
				{
					fread(&temp, 1, 1, f3);
				}
				else
				{
					int nread = fread(buff + j, 1, 1, f3);
					j++;
					if (nread <= 0)
						flag = 1;
				}
				break;
			case 3:
				if (i == parity_index)
				{
					fread(&temp, 1, 1, f4);
				}
				else
				{
					int nread = fread(buff + j, 1, 1, f4);
					j++;
					if (nread <= 0)
						flag = 1;
				}
				break;
			case 4:
				if (i == parity_index)
				{
					fread(&temp, 1, 1, f5);
				}
				else
				{
					int nread = fread(buff + j, 1, 1, f5);
					j++;
					if (nread <= 0)
						flag = 1;
				}
				break;
			default:
				break;
			}
		}
		if (flag == 0)
			fwrite(buff, 1, 4, fp);
		else
		{
			j--;
			if (j != 0)
				fwrite(buff, 1, j, fp);
			break;
		}
		parity_index = (parity_index + 1) % n;
	}
	fclose(fp);
	fclose(f1);
	fclose(f2);
	fclose(f3);
	fclose(f4);
	fclose(f5);
	cout << "file extracted successfully" << endl;
}

