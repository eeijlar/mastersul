 /*
 * FileFilter.java
 *
 * Created on 19 June 2003, 15:26
 */

import javax.swing.filechooser.*;
import java.io.File;


public class CustomFileFilter extends FileFilter {


String[] extensions;
String description;

public CustomFileFilter(String ext) {
	this(new String[] {ext}, null);
}


public CustomFileFilter(String[] exts, String descr) {
	extensions = new String[exts.length];
	for(int i=exts.length - 1; i >= 0; i--) {
		extensions[i]=exts[i].toLowerCase();
	}

	description=(descr==null ? exts[0] + " files" : descr);

}

public boolean accept(File f) {
	if(f.isDirectory()) { return true;}

	String name= f.getName().toLowerCase();
	for(int i=extensions.length-1; i>=0; i--) {
		if(name.endsWith(extensions[i])) {
			return true;
		}
	}
	return false;
}
public String getDescription () { return description; }

}