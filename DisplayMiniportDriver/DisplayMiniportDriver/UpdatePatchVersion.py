# import xml.etree.ElementTree as ET

# ET.register_namespace('', 'http://schemas.microsoft.com/developer/msbuild/2003')
# tree = ET.parse('Version.props')
# root = tree.getroot()

# for patchVersion in root.iter('{http://schemas.microsoft.com/developer/msbuild/2003}PATCH_VERSION'):
#     newVersion = int(patchVersion.text) + 1
#     print("Updated version to {}".format(newVersion))
#     patchVersion.text = str(newVersion)


# tree.write("Version.props", encoding='utf-8', xml_declaration=True)

def read_version(filename):
    try:
        with open(filename, 'r') as file:
            version = int(file.read())
            return version
    except FileNotFoundError:
        print("File not found.")
        return None
    except ValueError:
        print("Invalid content in file. Please make sure it contains a valid number.")
        return None

def write_version(filename, version):
    try:
        with open(filename, 'w') as file:
            file.write(str(version))
    except IOError:
        print("Error writing to file.")

def increment_version(filename):
    current_version = read_version(filename)
    if current_version is not None:
        new_version = current_version + 1
        write_version(filename, new_version)
        print(f"Version incremented. New version: {new_version}")

# File name
file_name = "../version.txt"

# Increment version
increment_version(file_name)