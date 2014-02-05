This folder contain automated unit tests

## How to add a new test

* Copy `__TEMPLATE__.c` to your own test file, says `test_string.c`.

* Change `Makefile`:

    List `test_string.c` with all **.c** files needed to run the test. No **.h**
    file is required.

    ```
    TEST_STRING_SRC = \
    string.c \
    test_string.c \
    $(UNITTEST_SRC)
    ```

    Add a rule for your test executable

    ```
    test_string: $(TEST_STRING_SRC)
    ```

    Append `test_string` to the end of the prerequisite list for the `test` rule.
