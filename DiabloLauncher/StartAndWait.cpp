#include "pch.h"
#include "StartAndWait.h"
#include <exception>

namespace {
	class LaunchException : std::exception {
		LaunchCode err = LaunchCode::Ok;

	public:
		LaunchException(LaunchCode err) : std::exception(), err(err) {}
		LaunchCode getErr() const { return err; }
	};
}
LaunchCode startAndWait(LPCWSTR path) {
	STARTUPINFOW si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	BOOL cr;
	LaunchCode ret = LaunchCode::Ok;

	try {
		HANDLE hJob = CreateJobObjectW(NULL, NULL);

		if (hJob == INVALID_HANDLE_VALUE) {
			throw LaunchException(LaunchCode::JobCreation);
		}
		try {
			HANDLE ioCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL,
				0, 1);
			if (ioCP == INVALID_HANDLE_VALUE) {
				throw LaunchException(LaunchCode::IoCompletionPortCreation);
			}
			JOBOBJECT_ASSOCIATE_COMPLETION_PORT port = { hJob, ioCP };
			cr = SetInformationJobObject(hJob, JobObjectAssociateCompletionPortInformation,
				&port, sizeof(port));
			if (!cr) {
				throw LaunchException(LaunchCode::JobAssociation);
			}

			try {
				cr = CreateProcessW(path, NULL, NULL, NULL, FALSE,
					CREATE_SUSPENDED, NULL, NULL, &si, &pi);
				if (!cr) {
					throw LaunchException(LaunchCode::ProcessCreation);
				}
				cr = AssignProcessToJobObject(hJob, pi.hProcess);
				if (cr) ResumeThread(pi.hThread);
				CloseHandle(pi.hThread);
				CloseHandle(pi.hProcess);
				if (!cr) {
					LPTSTR msg;
					FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
						NULL, 0, 0, (LPTSTR)&msg, 0, NULL);
					throw LaunchException(LaunchCode::ProcessAssign);
				}

				DWORD code;
				ULONG_PTR key;
				LPOVERLAPPED overlapped;

				while (GetQueuedCompletionStatus(ioCP, &code,
					&key, &overlapped, INFINITE)) {
					if ((HANDLE)key == hJob && code == JOB_OBJECT_MSG_ACTIVE_PROCESS_ZERO) {
						break;
					}
				}
			}
			catch (LaunchException&) {
				CloseHandle(ioCP);
				throw;
			}
			CloseHandle(ioCP);
		}
		catch (LaunchException&) {
			CloseHandle(hJob);
			throw;
		}
		CloseHandle(hJob);
	}
	catch (LaunchException& exc) {
		ret = exc.getErr();
	}
	return ret;
}