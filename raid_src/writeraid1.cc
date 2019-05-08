#include <bits/stdc++.h>
#include <string>
#include "writeraid1.h"
#include <experimental/filesystem>

using namespace std;

void write(string filepath)
{

	int parity_index = 0, n = 5;
	unsigned char parity;
	string destination_filepath1 = "/home/nthanhtin/220/raid/raid_files/disk1";
	string destination_filepath2 = "/home/nthanhtin/220/raid/raid_files/disk2";
	string destination_filepath3 = "/home/nthanhtin/220/raid/raid_files/disk3";
	string destination_filepath4 = "/home/nthanhtin/220/raid/raid_files/disk4";
	string destination_filepath5 = "/home/nthanhtin/220/raid/raid_files/disk5";

	string destination_filepath1c = "/home/nthanhtin/220/raid/raid_files/disk1c";
	string destination_filepath2c = "/home/nthanhtin/220/raid/raid_files/disk2c";
	string destination_filepath3c = "/home/nthanhtin/220/raid/raid_files/disk3c";
	string destination_filepath4c = "/home/nthanhtin/220/raid/raid_files/disk4c";
	string destination_filepath5c = "/home/nthanhtin/220/raid/raid_files/disk5c";

	FILE *fp = fopen(filepath.c_str(), "rb");
	FILE *f1 = fopen(destination_filepath1.c_str(), "wb");
	FILE *f2 = fopen(destination_filepath2.c_str(), "wb");
	FILE *f3 = fopen(destination_filepath3.c_str(), "wb");
	FILE *f4 = fopen(destination_filepath4.c_str(), "wb");
	FILE *f5 = fopen(destination_filepath5.c_str(), "wb");
	if (fp == NULL)
	{
		printf("File open error");
		return;
	}
	cout << endl;

	while (1)
	{
		unsigned char buff[4] = {0,0,0,0};
		int nread = fread(buff, 1, 4, fp);

		if (nread > 0)
		{
			for (int k = 0; k < nread; k++)
				parity = parity ^ buff[k];
			int j = 0;
			for (int i = 0; i < n && nread != j; i++)
			{
				switch (i)
				{
				case 0:
					if (i == parity_index)
					{
						fwrite(&parity, 1, 1, f1);
					}
					else
					{
						fwrite(buff + j, 1, 1, f1);
						j++;
					}
					break;
				case 1:
					if (i == parity_index)
					{
						fwrite(&parity, 1, 1, f2);
					}
					else
					{
						fwrite(buff + j, 1, 1, f2);
						j++;
					}
					break;
				case 2:
					if (i == parity_index)
					{
						fwrite(&parity, 1, 1, f3);
					}
					else
					{
						fwrite(buff + j, 1, 1, f3);
						j++;
					}
					break;
				case 3:
					if (i == parity_index)
					{
						fwrite(&parity, 1, 1, f4);
					}
					else
					{
						fwrite(buff + j, 1, 1, f4);
						j++;
					}
					break;
				case 4:
					if (i == parity_index)
					{
						fwrite(&parity, 1, 1, f5);
					}
					else
					{
						fwrite(buff + j, 1, 1, f5);
						j++;
					}
					break;
				default:
					break;
				}
			}

			if (parity_index >= j)
			{
				switch (parity_index)
				{
				case 1:
					fwrite(&parity, 1, 1, f2);
					break;
				case 2:
					fwrite(&parity, 1, 1, f3);
					break;
				case 3:
					fwrite(&parity, 1, 1, f4);
					break;
				case 4:
					fwrite(&parity, 1, 1, f5);
					break;
				}
			}

			parity_index = (parity_index + 1) % n;
		}
		if (nread < 4)
		{
			if (feof(fp))
				printf("file saved\n");
			if (ferror(fp))
				printf("Error reading\n");
			break;
		}
	}

	fclose(fp);
	fclose(f1);
	fclose(f2);
	fclose(f3);
	fclose(f4);
	fclose(f5);
	
	experimental::filesystem::copy(destination_filepath1, destination_filepath1c);
	experimental::filesystem::copy(destination_filepath2, destination_filepath2c);
	experimental::filesystem::copy(destination_filepath3, destination_filepath3c);
	experimental::filesystem::copy(destination_filepath4, destination_filepath4c);
	experimental::filesystem::copy(destination_filepath5, destination_filepath5c);

}

