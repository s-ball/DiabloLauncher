#include "pch.h"

enum class LaunchCode {
	Ok = 0,
	JobCreation,
	IoCompletionPortCreation,
	JobAssociation,
	ProcessCreation,
	ProcessAssign,
};

LaunchCode startAndWait(LPCWSTR path);