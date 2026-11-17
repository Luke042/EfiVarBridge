#include <Uefi.h>
#include "print.h"

#pragma warning(disable : 5208)

EFI_RUNTIME_SERVICES* gRT = 0;
EFI_BOOT_SERVICES* gBS = 0;

EFI_EXIT_BOOT_SERVICES orig_ebs = 0;

EFI_GUID vendor_guid = { 0x4F4E5958, 0x58B7, 0x4940, { 0x9C, 0x72, 0x12, 0x26, 0x39, 0x05, 0x18, 0xF8 } };
EFI_GUID loaded_image_protocol = { 0x4F4E5958, 0x58B7, 0x4940, { 0x9C, 0x72, 0x12, 0x26, 0x39, 0x05, 0x18, 0xF8 } };

EFI_STATUS EFIAPI hooked_ebs(EFI_HANDLE image_handle, UINTN map_key)
{
	// Create a EFI variable
	CHAR16* var_name = L"EFB";
	UINT32 value = 1337;

	const EFI_STATUS status = gRT->SetVariable(var_name, &vendor_guid, EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS, sizeof(value), &value);
	const CHAR16* status_string = (status != 0) ? L"Succeeded" : L"Failed";

	if (EFI_ERROR(status))
	{
		print(L"[-] Failed to set variable.\nStatus: %c\n", status_string);
		return status;
	}

	print(L"[+] Succesfully set variable.\nStatus %c\n", status_string);

	//Restore original EBS context and return original function
	gBS->ExitBootServices = orig_ebs;
	return orig_ebs(image_handle, map_key);
}

EFI_STATUS EfiMain(EFI_HANDLE image_handle, EFI_SYSTEM_TABLE* system_table)
{
	// Simple stuff setting up required pointers and cache the original EBS pointer
	gRT = system_table->RuntimeServices;
	gBS = system_table->BootServices;
	c_system_table = system_table;
	orig_ebs = gBS->ExitBootServices;

	print(L"[*] Loaded Efi...\n[*] Attempting to patch EBS\n");

	// Make EBS point to our function and check if it worked
	gBS->ExitBootServices = hooked_ebs;
	if (gBS->ExitBootServices != hooked_ebs)
	{
		print(L"[-] Failed to apply hook.\n");
		return EFI_UNSUPPORTED;
	}
		
	print(L"[+] EBS hook applied succesfully.\n");
	print(L"[+] Original EBS: 0x%p\n[+] Hooked EBS: 0x%p\r\n", (void*)orig_ebs, (void*)hooked_ebs);

	return EFI_SUCCESS;
}