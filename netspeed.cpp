#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#include "winsock2.h"
#include "ws2tcpip.h"
#include "iphlpapi.h"
#include "windef.h"

#include "netspeed.h"

#pragma comment(lib ,"Iphlpapi.lib")

#define UdPrint printf

BOOL WINAPI QueryNetworkTransmissionRate(PNETWORK_TRANSMISSION_INFO lpNetworkTransmissionInfo)
{
	BOOL bQueryResult = FALSE;
	if (lpNetworkTransmissionInfo)
	{
		PMIB_IF_TABLE2 pNetworkTable = NULL;
		NTSTATUS ntResult = GetIfTable2(&pNetworkTable);
		if (ntResult == ERROR_SUCCESS)
		{
			for (DWORD dwIndex = 0; dwIndex < pNetworkTable->NumEntries; dwIndex++)
			{
				if (pNetworkTable->Table[dwIndex].InterfaceAndOperStatusFlags.ConnectorPresent)
				{
					lpNetworkTransmissionInfo->u64DownloadNumberOfByte += pNetworkTable->Table[dwIndex].InOctets;
					lpNetworkTransmissionInfo->u64UploadNumberOfByte += pNetworkTable->Table[dwIndex].OutOctets;
				}
			}
			bQueryResult = (pNetworkTable->NumEntries > 0);
			FreeMibTable(pNetworkTable);
			pNetworkTable = NULL;
		}
		else UdPrint("main.cpp::QueryNetworkTransmissionRate -> GetIfTable2 Error, Code = %d\n", ntResult);
	}
	else UdPrint("main.cpp::QueryNetworkTransmissionRate -> Params Value Invalid\n");
	return bQueryResult;
}
