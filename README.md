# Database-Management-System
A file-based Database Management System (DBMS) built from scratch in C++, designed to understand how databases work internally.
This project implements core DBMS concepts such as schema management, data persistence, command parsing, and basic query execution — without using any external database libraries.

Each table is represented using:
.schema file for structure (metadata)
.data file for actual records

Features

✅ Create tables with a dynamic schema
✅ Store table structure separately using schema files
✅ Insert records with schema-based data type validation
✅ Persistent storage using files (data survives program restarts)
✅ Display table data in a clean tabular format
✅ Select records with conditional filtering (WHERE clause)
✅ Delete records based on conditions (safe file rewrite)
✅ Drop entire tables (schema + data)
✅ Robust handling of invalid inputs and syntax errors
