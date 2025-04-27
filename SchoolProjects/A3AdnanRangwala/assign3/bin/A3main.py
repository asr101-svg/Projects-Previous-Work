from asciimatics.widgets import *
from asciimatics.scene import *
from asciimatics.screen import *
from asciimatics.exceptions import *
from time import sleep
from ctypes import *
import os
import mysql.connector
from datetime import datetime

testLibPath = './libvcparser.so'

conn = None
cursor = None
selected_item = None

createQuery = "create table if not exists FILE (file_id int auto_increment primary key,  file_name VARCHAR(60) not null,  last_modified datetime, creation_time datetime not null)"
createQuery2 = "create table if not exists CONTACT (contact_id int auto_increment primary key,  name VARCHAR(256) not null,  birthday datetime, anniversary datetime, file_id int not null, FOREIGN KEY (file_id) REFERENCES FILE(file_id) ON DELETE CASCADE)"

# Create a reference to the library module
testlib = CDLL(testLibPath)

# Define the VCardErrorCode enum
class VCardErrorCode(c_int):
    OK = 0
    INV_FILE = 1
    INV_CARD = 2
    INV_PROP = 3
    INV_DT = 4
    OTHER_ERROR = 5

class Property(Structure):
    _fields_ = [("name", c_char_p),
                ("group", c_char_p),
                ("parameters", POINTER(c_void_p)),
                ("values", POINTER(c_void_p))]

class DateTime(Structure):
    _fields_ = [
                ("isUTC", c_bool),
                ("isText", c_bool),
                ("date", c_char_p),
                ("time", c_char_p),
                ("text", c_char_p),
                ]

class Card(Structure):
    _fields_ = [("fn", POINTER(Property)),
                ("optionalProperties", POINTER(c_void_p)),
                ("birthday", POINTER(DateTime)),
                ("anniversary", POINTER(DateTime))]

testlib.createCard.argtypes = [c_char_p, POINTER(POINTER(Card))]
testlib.createCard.restype = c_int

testlib.updateCardFn.argtypes = [POINTER(Card), c_char_p]
testlib.updateCardFn.restype = None

testlib.validateCard.argtypes = [POINTER(Card)]
testlib.validateCard.restype = c_int

testlib.writeCard.argtypes = [c_char_p, POINTER(Card)]
testlib.writeCard.restype = c_int

testlib.getOptLength.argtypes = [POINTER(Card)]
testlib.getOptLength.restype = c_int

testlib.createCardUI.argtypes = [c_char_p, POINTER(POINTER(Card))]
testlib.createCardUI.restype = None

class LoginScreen(Frame):
    def __init__(self, screen):
        super(LoginScreen, self).__init__(screen, screen.height, screen.width, has_border=False, title="Login")
        self._screen = screen
        layout = Layout([100], fill_frame=False)
        self.add_layout(layout)
        layout.add_widget(Label("Login", align="^"))
        self.fix()

        layout = Layout([1, 1], fill_frame=False)
        self.add_layout(layout)
        layout.add_widget(Label("Username:"), 0)
        self.username = Text(name="username")
        self.username.value = ""
        layout.add_widget(self.username, 1)

        layout.add_widget(Label("Password:"), 0)
        self.password = Text(name="password")
        self.password.value = ""
        layout.add_widget(self.password, 1)

        layout.add_widget(Label("DB Name:"), 0)
        self.dbName = Text(name="DBname")
        self.dbName.value = ""
        layout.add_widget(self.dbName, 1)

        layout = Layout([1, 1], fill_frame=False)
        self.add_layout(layout)
        layout.add_widget(Button("Login", self._login), 0)
        layout.add_widget(Button("Exit", self._exit), 1)
        self.fix()

    def _login(self):
        global conn, cursor
        username = self.username.value
        password = self.password.value
        dbName = self.dbName.value
        try:
            conn = mysql.connector.connect(host="dursley.socs.uoguelph.ca", database=dbName, user=username, password=password)
            conn.autocommit = True
            cursor = conn.cursor()
            cursor.execute(createQuery)
            cursor.execute(createQuery2)
            raise NextScene("MainMenu")
        except mysql.connector.Error as err:
            self._screen.clear()
            self._screen.print_at(str(err), 0, 0)
            self._screen.refresh()
            sleep(2)

    def _exit(self):
        raise StopApplication("User requested exit")

class VCardManager(Frame):
    def __init__(self):
        pass


    def create_card_ui(self, contact_name):
        card_ptr = POINTER(Card)()
        card_ptr_ptr = POINTER(POINTER(Card))(card_ptr)
        testlib.createCardUI(contact_name.encode('utf-8'), card_ptr_ptr)
        return card_ptr.contents

    def create_card(self, file_name):
        card_ptr = POINTER(Card)()
        card_ptr_ptr = POINTER(POINTER(Card))(card_ptr)
        fullFileName = ("cards/" + file_name)
        result = testlib.createCard(fullFileName.encode('utf-8'), card_ptr_ptr)
        if result != VCardErrorCode.OK:
            error_messages = {
                VCardErrorCode.INV_FILE: "Invalid file",
                VCardErrorCode.INV_CARD: "Invalid card",
                VCardErrorCode.INV_PROP: "Invalid property",
                VCardErrorCode.INV_DT: "Invalid date/time",
                VCardErrorCode.OTHER_ERROR: "Other error"
            }
            error_message = error_messages.get(result, "Unknown error")
            raise Exception(f"Error creating card: {error_message} (code: {result})")
        return card_ptr.contents

    def write_card(self, file_name, card_ptr):
        fullFileName = ("cards/" + file_name)
        result = testlib.writeCard(fullFileName.encode('utf-8'), card_ptr)
        if result != VCardErrorCode.OK:
            error_messages = {
                VCardErrorCode.INV_FILE: "Invalid file",
                VCardErrorCode.INV_CARD: "Invalid card",
                VCardErrorCode.INV_PROP: "Invalid property",
                VCardErrorCode.INV_DT: "Invalid date/time",
                VCardErrorCode.OTHER_ERROR: "Other error"
            }
            error_message = error_messages.get(result, "Unknown error")
            raise Exception(f"Error writing card: {error_message} (code: {result})")

    def update_card_fn(self, card_ptr, new_fn):
        testlib.updateCardFn(card_ptr, new_fn.encode('utf-8'))

    def validate_card(self, card_ptr):
        result = testlib.validateCard(card_ptr)
        if result != VCardErrorCode.OK:
            error_messages = {
                VCardErrorCode.INV_FILE: "Invalid file",
                VCardErrorCode.INV_CARD: "Invalid card",
                VCardErrorCode.INV_PROP: "Invalid property",
                VCardErrorCode.INV_DT: "Invalid date/time",
                VCardErrorCode.OTHER_ERROR: "Other error"
            }
            error_message = error_messages.get(result, "Unknown error")
            raise Exception(f"Error validating card: {error_message} (code: {result})")
        return result

    def get_optional_length(self, card_ptr):
        return testlib.getOptLength(card_ptr)

    def file_name_exists(self, file_name):
        global cursor
        query = "SELECT COUNT(*) FROM FILE WHERE file_name = %s"
        cursor.execute(query, (file_name,))
        result = cursor.fetchone()
        return result[0] > 0

    def get_file_id(self, file_name):
        global cursor
        query = "SELECT file_id FROM FILE WHERE file_name = %s"
        cursor.execute(query, (file_name,))
        result = cursor.fetchone()
        if result:
            return result[0]
        else:
            return None
    
    def get_file_created(self, file_name, file_id):
        global cursor
        query = "SELECT creation_time FROM FILE WHERE file_name = %s and file_id = %s"
        cursor.execute(query, (file_name,file_id))
        result = cursor.fetchone()
        if result:
            return result[0]
        else:
            return None

    def check_for_contact_dupe(self, name, file_id,bdayDateTime,anniDateTime):
        global cursor
        bDayExists = True
        anniExists = True
        if bdayDateTime == "" or bdayDateTime == None or bdayDateTime == "null":
            bDayExists = False
        if anniDateTime == "" or anniDateTime == None or anniDateTime == "null": 
            # screen.clear()
            # screen.print_at("anniDateTime is false", 0, 0)
            # screen.refresh()
            # sleep(5)
            anniExists = False
        try:
            if bDayExists == True and anniExists == True:
                query = "SELECT COUNT(*) FROM CONTACT WHERE name = %s AND file_id = %s and birthday = %s and anniversary = %s"
                cursor.execute(query, (name, file_id, bdayDateTime, anniDateTime))
            elif anniExists == True:
                query = "SELECT COUNT(*) FROM CONTACT WHERE name = %s AND file_id = %s and anniversary = %s and birthday IS NULL"
                cursor.execute(query, (name, file_id, anniDateTime))
                # screen.clear()
                # screen.print_at((query,name,file_id,anniDateTime), 0, 0)
                # screen.refresh()
                # sleep(5)
            elif bDayExists == True:
                query = "SELECT COUNT(*) FROM CONTACT WHERE name = %s AND file_id = %s and birthday = %s and anniversary IS NULL"
                cursor.execute(query, (name, file_id, bdayDateTime))
            else:
                query = "SELECT COUNT(*) FROM CONTACT WHERE name = %s AND file_id = %s AND birthday IS NULL AND anniversary IS NULL"
                cursor.execute(query, (name, file_id))
                # screen.clear()
                # screen.print_at((query,name,file_id,anniDateTime), 0, 0)
                # screen.refresh()
                # sleep(5)
        except mysql.connector.Error as err:
            exit()
        result = cursor.fetchone()
        # screen.clear()
        # screen.print_at("for when name = " + name + " and file_id = " + str(file_id) + " and birthday = " + str(bdayDateTime) + " and anni = " + str(anniDateTime)  +" There are: " + str(result), 0, 0)
        # screen.refresh()
        # sleep(5)
        return result[0] > 0

    def add_to_database(self, card_ptr, file_name):
        global cursor
        fn = cast(card_ptr.fn.contents.values.contents, POINTER(c_char_p)).contents.value.decode('utf-8') if card_ptr.fn else ""
        birthdayDate = ""
        birthdayTime = ""
        anniversaryDate = ""
        anniversaryTime = ""
        bdayDateTime = None
        anniDateTime = None
        fileID = self.get_file_id(file_name)
        if card_ptr.birthday:
            if card_ptr.birthday.contents.isText == False:
                birthdayDate = card_ptr.birthday.contents.date.decode('utf-8')
                birthdayTime = card_ptr.birthday.contents.time.decode('utf-8')
                if birthdayDate != "" or birthdayTime != "":
                    bdayDateTime = self.parse_vcard_date_time(birthdayDate, birthdayTime)
            else :
                bdayDateTime = "null"
        if card_ptr.anniversary:
            if card_ptr.anniversary.contents.isText == False:
                anniversaryDate = card_ptr.anniversary.contents.date.decode('utf-8')
                anniversaryTime = card_ptr.anniversary.contents.time.decode('utf-8')
                if anniversaryDate != "" or anniversaryTime != "":
                    anniDateTime = self.parse_vcard_date_time(anniversaryDate, anniversaryTime)
            else :
                anniDateTime = "null"
        
        if not self.check_for_contact_dupe(fn, fileID, bdayDateTime, anniDateTime):
            if bdayDateTime == "null" and anniDateTime == "null":
                query = "INSERT INTO CONTACT (name,file_id) VALUES (%s, %s)"
                try:
                    cursor.execute(query, (fn, bdayDateTime, anniDateTime, fileID))
                except mysql.connector.Error as err:
                    exit()
            elif bdayDateTime == "null":
                query = "INSERT INTO CONTACT (name, anniversary, file_id) VALUES (%s, %s, %s)"
                try:
                    cursor.execute(query, (fn, anniDateTime, fileID))
                except mysql.connector.Error as err:
                    exit()
            elif anniDateTime == "null":
                query = "INSERT INTO CONTACT (name, birthday, file_id) VALUES (%s, %s, %s)"
                try:
                    cursor.execute(query, (fn, bdayDateTime, fileID))
                except mysql.connector.Error as err:
                    exit()
            else:
                query = "INSERT INTO CONTACT (name, birthday, anniversary, file_id) VALUES (%s, %s, %s, %s)"
                try:
                    cursor.execute(query, (fn, bdayDateTime, anniDateTime, fileID))
                except mysql.connector.Error as err:
                    exit()

    def parse_vcard_date_time(self, date_str, time_str):
        # Parse the date and time strings
        date = datetime.strptime(date_str, "%Y%m%d")
        time = datetime.strptime(time_str, "%H%M%S").time()
        # Combine date and time into a single datetime object
        combined_datetime = datetime.combine(date, time)
        # Format the datetime object as a string in the SQL DATETIME format
        return combined_datetime.strftime("%Y-%m-%d %H:%M:%S")

    def update_file_database(self,file_name):
        global cursor
        fileID = self.get_file_id(file_name)
        fileCreated = self.get_file_created(file_name, fileID)
        fileModified = datetime.fromtimestamp(os.path.getmtime('cards/' + file_name))
        # self._screen.clear()
        # self._screen.print_at("File ID: " + str(fileID) + " File Created: " + str(fileCreated), 0, 0)
        # self._screen.refresh()
        # sleep(10)
        query = "UPDATE FILE SET last_modified = %s WHERE file_id = %s and file_name = %s"
        cursor.execute(query,(fileModified,fileID,file_name))

class DBQueries(Frame): 
    def __init__(self, screen):
        super(DBQueries, self).__init__(screen, screen.height, screen.width, has_border=False, title="DB Queries")
        self._screen = screen
        global cursor
        layout = Layout([100], fill_frame=False)
        self.add_layout(layout)
        self.textBox = TextBox(10, as_string=True, name="query",readonly = True)
        # try:
        #     cursor.execute("SELECT * FROM FILE")
        #     result = cursor.fetchall()
        #     result_str = "\n".join([str(row) for row in result])
        # except mysql.connector.Error as err:
        #     exit()
        # self.fix()
        # self.textBox.value = result_str
        layout.add_widget(self.textBox)
        
        
        buttonLayout = Layout([1, 1, 1], fill_frame=False)
        self.add_layout(buttonLayout)
        buttonLayout.add_widget(Button("Display all Contacts", self._showAll), 0)
        buttonLayout.add_widget(Button("Find Contacts Born in June", self._june), 1)
        buttonLayout.add_widget(Button("Cancel", self._cancel), 2)
        self.fix()

    def _showAll(self):
        global cursor
        self._screen.clear()
        self._screen.print_at("You entered: show all", 0, 0)
        self._screen.refresh()
        sleep(1)
        try:
            cursor.execute("select contact_id,name,birthday,anniversary,file_name from CONTACT,FILE where CONTACT.file_id = FILE.file_id order by name")
            result = cursor.fetchall()
            result_str = "\n".join([str(row) for row in result])
        except mysql.connector.Error as e:
            self._screen.clear()
            self._screen.print_at("Error with query: " + str(e), 0, 0)
            self._screen.refresh()
            sleep(5)
            return
        
        self.textBox.value = result_str
        self.fix()

    
    def _june(self):
        global cursor
        self._screen.clear()
        self._screen.print_at("You entered: show all in june", 0, 0)
        self._screen.refresh()
        sleep(1)

        try:
            cursor.execute("select name,birthday from CONTACT,FILE where CONTACT.file_id = FILE.file_id and MONTH(CONTACT.birthday) = 6 order by DATEDIFF(CONTACT.birthday,FILE.last_modified)")
            resultJune = cursor.fetchall()
            result_str = "\n".join([str(row) for row in resultJune])
        except mysql.connector.Error as e:
            self._screen.clear()
            self._screen.print_at("Error with query: " + str(e), 0, 0)
            self._screen.refresh()
            sleep(5)
            return

        self.textBox.value = result_str
        self.fix()
        

    def _cancel(self):
        raise NextScene("MainMenu")

class vCardCreate(Frame):
    def __init__(self, screen):
        super(vCardCreate, self).__init__(screen, screen.height, screen.width, has_border=False, title="vCard List")
        self._screen = screen
        self.card_ptr = None
        manager = VCardManager()     

        layout1 = Layout([100],fill_frame =False)
        self.add_layout(layout1)
        layout1.add_widget(Label("vCard Details", align="^"))

        layout = Layout([1,1], fill_frame=False)
        self.add_layout(layout)
        
        layout.add_widget(Label("File Name:"),0)
        self.file_name_box = Text(name="file_name")
        self.file_name_box.value = "enter file here"
        layout.add_widget(self.file_name_box,1)

        layout.add_widget(Label("Contact Name:"),0)
        self.contact_name_box = Text(name="contact_name")
        self.contact_name_box.value = "enter fn here"
        layout.add_widget(self.contact_name_box,1)

        layout.add_widget(Label("Birthday Date:"),0)
        layout.add_widget(Label(""),1)

        layout.add_widget(Label("Birthday Time:"),0)
        layout.add_widget(Label(""),1)
        

        layout.add_widget(Label("Anniversary Date:"),0)
        layout.add_widget(Label(""),1)

        layout.add_widget(Label("Anniversary Time:"),0)
        layout.add_widget(Label(""),1)

        layout.add_widget(Label("Optional Properties:"),0)
        layout.add_widget(Label(0),1)
        
        self.fix()

        buttonLayout = Layout([1, 1], fill_frame=False)
        self.add_layout(buttonLayout)
        buttonLayout.add_widget(Button("Ok", self._ok), 0)
        buttonLayout.add_widget(Button("Cancel", self._cancel), 1)
        self.fix()

    def _ok(self):
        global cursor
        contactName = self.contact_name_box.value 
        if contactName == "":
            self._screen.clear()
            self._screen.print_at("Contact Name is required", 0, 0)
            self._screen.refresh()
            sleep(1)
            return
        manager = VCardManager()
        self.card_ptr = manager.create_card_ui(contactName)
        fn = cast(self.card_ptr.fn.contents.values.contents, POINTER(c_char_p)).contents.value.decode('utf-8')
        # self._screen.clear()
        # self._screen.print_at("Contact Name is " + fn, 0, 0)
        # self._screen.refresh()
        # sleep(10)
        try:
            manager.validate_card(self.card_ptr)
            fn = cast(self.card_ptr.fn.contents.values.contents, POINTER(c_char_p)).contents.value.decode('utf-8')
            # self._screen.clear()
            # self._screen.print_at("After validate, Contact Name is " + fn, 0, 0)
            # self._screen.refresh()
            # sleep(10)
        except Exception as e:
            self._screen.print_at(str(e), 0, 0)
            self._screen.refresh()
            sleep(2)
            return
        if self.file_name_box.value.endswith(".vcf"):
            try:
                manager.write_card(self.file_name_box.value, self.card_ptr)
                if manager.get_file_id(self.file_name_box.value) != None:
                    manager.update_file_database(self.file_name_box.value)
                else:
                    fileModified = datetime.fromtimestamp(os.path.getmtime('cards/' + self.file_name_box.value))
                    query = "INSERT INTO FILE (file_name, last_modified, creation_time) VALUES (%s, %s, NOW())"
                    cursor.execute(query,(self.file_name_box.value,fileModified))
                manager.add_to_database(self.card_ptr, self.file_name_box.value)
                
                fn = cast(self.card_ptr.fn.contents.values.contents, POINTER(c_char_p)).contents.value.decode('utf-8')
                # self._screen.clear()
                # self._screen.print_at("After write card, Contact Name is " + fn, 0, 0)
                # self._screen.refresh()
                # sleep(10)
            except Exception as e:
                self._screen.print_at(str(e), 0, 0)
                self._screen.refresh()
                sleep(2)
                return
        else:
            self._screen.clear()
            self._screen.print_at("Invalid file!", 0, 0)
            self._screen.refresh()
            sleep(2)
            return
        
        self._screen.clear()
        self._screen.print_at("Successfully created Card", 0, 0)
        self._screen.refresh()
        sleep(1)

    def _cancel(self):
        raise NextScene("MainMenu")

class vCardEdit(Frame):
    def __init__(self, screen):
        super(vCardEdit, self).__init__(screen, screen.height, screen.width, has_border=False, title="vCard List")
        self._screen = screen
        self.card_ptr = None
        global selected_item

        layout1 = Layout([100],fill_frame =False)
        self.add_layout(layout1)
        layout1.add_widget(Label("vCard Details", align="^"))

        layout = Layout([1,1], fill_frame=False)
        self.add_layout(layout)
        
        layout.add_widget(Label("File Name:"),0)
        self.file_name_box = Text(name="file_name")
        self.file_name_box.value = selected_item
        layout.add_widget(self.file_name_box,1)

        layout.add_widget(Label("Contact Name:"),0)
        self.contact_name_box = Text(name="contact_name")
        layout.add_widget(self.contact_name_box,1)

        layout.add_widget(Label("Birthday Date:"),0)
        self.birthday_date_label = Label("")
        layout.add_widget(self.birthday_date_label,1)

        # layout.add_widget(Label("Birthday Time:"),0)
        # self.birthday_time_label = Label("")
        # layout.add_widget(self.birthday_time_label,1)
        

        layout.add_widget(Label("Anniversary Date:"),0)
        self.anniversary_date_label = Label("")
        layout.add_widget(self.anniversary_date_label,1)

        # layout.add_widget(Label("Anniversary Time:"),0)
        # self.anniversary_time_label = Label("")
        # layout.add_widget(self.anniversary_time_label,1)

        layout.add_widget(Label("Optional Properties:"),0)
        self.optional_properties_label = Label("")
        layout.add_widget(self.optional_properties_label,1)
        
        self.fix()

        buttonLayout = Layout([1, 1], fill_frame=False)
        self.add_layout(buttonLayout)
        buttonLayout.add_widget(Button("Ok", self._ok), 0)
        buttonLayout.add_widget(Button("Cancel", self._cancel), 1)
        self.fix()

    def reset(self):
        self._load_vcard_data()
        super(vCardEdit, self).reset()
        
        
    def _load_vcard_data(self):
        global selected_item
        try:
            manager = VCardManager()
            self.card_ptr = manager.create_card(selected_item)
            fn = cast(self.card_ptr.fn.contents.values.contents, POINTER(c_char_p)).contents.value.decode('utf-8') if self.card_ptr.fn else ""
            optionalProperties = manager.get_optional_length(self.card_ptr)
            birthdayDate = ""
            birthdayTime = ""
            if self.card_ptr.birthday:
                if self.card_ptr.birthday.contents.isText == False:
                    birthdayDate = self.card_ptr.birthday.contents.date.decode('utf-8') if self.card_ptr.birthday else ""
                    birthdayTime = self.card_ptr.birthday.contents.time.decode('utf-8') if self.card_ptr.birthday else ""
                else:
                    birthdayDate = self.card_ptr.birthday.contents.text.decode('utf-8')
            else:
                birthdayDate = ""
                birthdayTime = ""
            anniversaryDate = ""
            anniversaryTime = ""
            if self.card_ptr.anniversary:
                if self.card_ptr.anniversary.contents.isText == False:
                    anniversaryDate = self.card_ptr.anniversary.contents.date.decode('utf-8') if self.card_ptr.anniversary else ""
                    anniversaryTime = self.card_ptr.anniversary.contents.time.decode('utf-8') if self.card_ptr.anniversary else ""
                else:
                    anniversaryDate = self.card_ptr.anniversary.contents.text.decode('utf-8')

                if self.card_ptr.anniversary.contents.isUTC == True:
                    anniversaryTime = anniversaryTime + " (UTC)"
            else:
                anniversaryDate = ""
                anniversaryTime = ""

            self.file_name_box.value = selected_item
            self.contact_name_box.value = fn
            self.birthday_date_label.text = birthdayDate + " " + birthdayTime
            # self.birthday_time_label.text = birthdayTime
            self.anniversary_date_label.text = anniversaryDate + " " + anniversaryTime
            # self.anniversary_time_label.text = anniversaryTime
            self.optional_properties_label.text = str(optionalProperties)

        except Exception as e:
            self._screen.clear()
            self._screen.print_at(str(e), 0, 0)
            self._screen.refresh()
            sleep(2)
            raise NextScene("MainMenu")
    
    def _ok(self):
        fn = cast(self.card_ptr.fn.contents.values.contents, POINTER(c_char_p)).contents.value.decode('utf-8')
        contactName = self.contact_name_box.value 
        if contactName == "":
            self._screen.clear()
            self._screen.print_at("Contact Name is required", 0, 0)
            self._screen.refresh()
            sleep(1)
            return
        manager = VCardManager()
        manager.update_card_fn(self.card_ptr, contactName)
        try:
            manager.validate_card(self.card_ptr)
        except Exception as e:
            self._screen.clear()
            self._screen.print_at(str(e), 0, 0)
            self._screen.refresh()
            sleep(2)
            manager.update_card_fn(self.card_ptr, fn)
            return
        if self.file_name_box.value.endswith(".vcf"):
            try:
                manager.write_card(self.file_name_box.value, self.card_ptr)
            except Exception as e:
                self._screen.clear()
                self._screen.print_at(str(e), 0, 0)
                self._screen.refresh()
                sleep(2)
                manager.update_card_fn(self.card_ptr, fn)
                return
            try:
                if manager.get_file_id(self.file_name_box.value) != None:
                    manager.update_file_database(self.file_name_box.value)
                    query = "UPDATE CONTACT SET name = %s WHERE file_id = %s"
                    cursor.execute(query,(contactName,manager.get_file_id(self.file_name_box.value)))
                else:
                    fileModified = datetime.fromtimestamp(os.path.getmtime('cards/' + self.file_name_box.value))
                    query = "INSERT INTO FILE (file_name, last_modified, creation_time) VALUES (%s, %s, NOW())"
                    cursor.execute(query,(self.file_name_box.value,fileModified))
                    manager.add_to_database(self.card_ptr, self.file_name_box.value)
                
                fn = cast(self.card_ptr.fn.contents.values.contents, POINTER(c_char_p)).contents.value.decode('utf-8')
                # self._screen.clear()
                # self._screen.print_at("After write card, Contact Name is " + fn, 0, 0)
                # self._screen.refresh()
                # sleep(10)
            except Exception as e:
                self._screen.clear()
                self._screen.print_at(str(e), 0, 0)
                self._screen.refresh()
                sleep(5)
                return
            
        else:
            self._screen.clear()
            self._screen.print_at("Invalid file!", 0, 0)
            self._screen.refresh()
            sleep(2)
            manager.update_card_fn(self.card_ptr, fn)
            return
        
        self._screen.clear()
        self._screen.print_at("Successfully edited Card", 0, 0)
        self._screen.refresh()
        sleep(1)

    def _cancel(self):
        raise NextScene("MainMenu")



class MainMenu(Frame):
    def __init__(self, screen):
        super(MainMenu, self).__init__(screen, screen.height, screen.width, has_border=False, title="vCard List")
        self._screen = screen
        layout = Layout([100], fill_frame=False)
        self.add_layout(layout)
        layout.add_widget(Label("Main Menu", align="^"))
        self.fix()
        global cursor

        # Layout for the list of files
        listLayout = Layout([100], fill_frame=True)
        self.add_layout(listLayout)
        self._listbox = ListBox(Widget.FILL_FRAME, [], name="list")
        listLayout.add_widget(self._listbox)
        self.fix()

        # Layout for the buttons
        buttonLayout = Layout([1, 1, 1, 1], fill_frame=False)
        self.add_layout(buttonLayout)
        buttonLayout.add_widget(Button("Create", self._create), 0)
        buttonLayout.add_widget(Button("Edit", self._edit), 1)
        buttonLayout.add_widget(Button("DB Queries", self._db), 2)
        buttonLayout.add_widget(Button("Exit", self._exit), 3)
        self.fix()

        

    def reset(self):
        super(MainMenu, self).reset()
        self._load_list()

    def _load_list(self):
        global cursor
        items = []
        for f in os.listdir('cards'):
            if os.path.isfile(os.path.join('cards', f)):
                manager = VCardManager()
                
                try:
                    card_ptr = manager.create_card(f)
                    manager.validate_card(card_ptr)
                    if not manager.file_name_exists(f):
                        last_modified = datetime.fromtimestamp(os.path.getmtime(os.path.join('cards', f)))
                        query = "INSERT INTO FILE (file_name, last_modified, creation_time) VALUES (%s, %s, NOW())"
                        cursor.execute(query, (f, last_modified))
                    items.append((f, f))
                except Exception as e:
                    # self._screen.clear()
                    # self._screen.print_at("Error occured: " + str(e), 0, 0)
                    # self._screen.refresh()
                    # sleep(10)
                    continue
                manager.add_to_database(card_ptr, f)
        self._listbox.options = items
        self.fix()

    def _create(self):
        raise NextScene("vCardCreate")

    def _edit(self):
        global selected_item
        selected_item = self._listbox.value
        if selected_item:
            raise NextScene("vCardEdit")
        else:
            self._screen.clear()
            self._screen.print_at("No item selected", 0, 0)
            self._screen.refresh()
            sleep(1)

    def _db(self):
        raise NextScene("DBQueries")

    @staticmethod
    def _exit():
        global cursor,conn
        cursor.close()
        conn.close()
        raise StopApplication("User requested exit")

def demo(screen,scene):
    global selected_item

    scenes = [
        Scene([LoginScreen(screen)], -1, name="LoginScreen"),
        Scene([MainMenu(screen)], -1, name="MainMenu"),
        Scene([vCardCreate(screen)], -1, name="vCardCreate"),
        Scene([vCardEdit(screen)], -1, name="vCardEdit"),
        Scene([DBQueries(screen)], -1, name="DBQueries"),
    ]
    screen.play(scenes, stop_on_resize=True, start_scene=scene, allow_int=True)

last_scene = None
while True:
    try:
        Screen.wrapper(demo, catch_interrupt=True, arguments=[last_scene])
        sys.exit(0)
    except ResizeScreenError as e:
        last_scene = e.scene