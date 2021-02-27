#include <Windows.h>
#include <iostream>
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>

#define DBG_PRINT() \
        { std::cout << "DBG_PRINT. LINE " << __LINE__ << std::endl; }

void showSQLError(uint32_t handleType, const SQLHANDLE& handle)
{
    SQLCHAR SQLState[1024];
    SQLCHAR message[1024];
    if (SQL_SUCCESS == SQLGetDiagRec(handleType, handle, 1, SQLState, nullptr, message, 1024, nullptr))
        std::cout << "SQL driver message: " << message << "\nSQL state: " << SQLState << std::endl;
}

void initSQL()
{
    SQLHANDLE SQLEnvHandle = nullptr;
    SQLHANDLE SQLConnectionHandle = nullptr;
    SQLHANDLE SQLStatementHandle = nullptr;
    SQLRETURN retCode = 0;
    char SQLQuery[] = "SELECT * FROM people";
    do {
        if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &SQLEnvHandle))
        {
            std::cout << "SQLAllocHandle failed!\n";
            break;
        }
        
        if (SQL_SUCCESS != SQLSetEnvAttr(SQLEnvHandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0))
            break;
       
        if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, SQLEnvHandle, &SQLConnectionHandle))
            break;
     
        if (SQL_SUCCESS != SQLSetConnectAttr(SQLConnectionHandle, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0))
            break;
       

        SQLCHAR retConString[1024];
        switch (SQLDriverConnect(SQLConnectionHandle, nullptr, (SQLCHAR*)"DRIVER={SQL Server}; SERVER=localhost, 1433; DATABASE=Test; UID=user0; PWD=123456789012345;", SQL_NTS, retConString, 1024, nullptr, SQL_DRIVER_NOPROMPT))
        {
        case SQL_SUCCESS:
        {
            break;
        }
        case SQL_SUCCESS_WITH_INFO:
        {
            break;
        }
        case SQL_NO_DATA_FOUND:
        {
            showSQLError(SQL_HANDLE_DBC, SQLConnectionHandle);
            retCode = -1;
            break;
        }
        case SQL_INVALID_HANDLE:
        {
            showSQLError(SQL_HANDLE_DBC, SQLConnectionHandle);
            retCode = -1;
            break;
        }
        case SQL_ERROR:
        {
            DBG_PRINT();
            showSQLError(SQL_HANDLE_DBC, SQLConnectionHandle);
            retCode = -1;
            break;
        }
        default:
        {
            std::cout << "Default case! Something went wrong ...\n";
            break;
        }
        }
        if (retCode == -1)
            break;
        /*if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, SQLConnectionHandle, &SQLStatementHandle))
            break;
        if (SQL_SUCCESS != SQLExecDirect(SQLStatementHandle, (SQLCHAR*)SQLQuery, SQL_NTS))
        {
            showSQLError(SQL_HANDLE_STMT, SQLStatementHandle);
            break;
        }
        else {
            char name[256];
            int age;
            while (SQLFetch(SQLStatementHandle) == SQL_SUCCESS) {
                SQLGetData(SQLStatementHandle, 1, SQL_C_DEFAULT, &name, sizeof(name), nullptr);
                SQLGetData(SQLStatementHandle, 2, SQL_C_DEFAULT, &age, sizeof(age), nullptr);
                std::cout << name << " " << age << std::endl;
            }
        }*/

    } while (FALSE);

    SQLFreeHandle(SQL_HANDLE_STMT, SQLStatementHandle);
    SQLDisconnect(SQLConnectionHandle);
    SQLFreeHandle(SQL_HANDLE_DBC, SQLConnectionHandle);
    SQLFreeHandle(SQL_HANDLE_ENV, SQLEnvHandle);
}
