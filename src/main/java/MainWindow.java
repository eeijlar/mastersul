 /*
 * MainWindow.java
 * Created: June 15th 2003
 *
 * Rev PA11 	Added radio buttons
 *				Removed check for file open from processButton Action listener
 *				Tidied up process button routine
 *
 * Rev PA12     Changed return type on native methods to void
 *
 * Rev PA13     Updated remove button action listener
 * Rev PA14 	Removed unused variables
 * Rev PA15     Updated settings and apply
 *
 */

import java.io.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.*;
import javax.swing.filechooser.*;
import java.io.File;
import javax.swing.text.*;
import javax.swing.border.*;
import javax.swing.colorchooser.*;
import javax.accessibility.*;
import java.applet.*;
import java.net.*;
import javax.sound.midi.*;
import javax.sound.sampled.*;


/**
 *
 * author  John Lawlor
 *
 *
 */

public class MainWindow extends JFrame implements FocusListener {

    /** Create new form MainWindow **/


    public MainWindow() {

		initComponents();
		initMenuBar();
		initToolBar();
		initMidiPane();
		initStatus();
        this.setSize(400,400);
    }

	/** Initialise frames **/

	private void initComponents() {

		setDefaultCloseOperation(DO_NOTHING_ON_CLOSE);
		setTitle("Project 3 - Music By Numbers");


		addWindowListener(new java.awt.event.WindowAdapter() {
		public void windowClosing(java.awt.event.WindowEvent evt) {
			// Exits despite NO option
			confirmExit();

		}
	    });
	}

	/** Initialise Menus **/

	private void initMenuBar() {

    	TitleBar =  new JMenuBar();
    	FileMenu =  new JMenu();
    	SettingsMenu = new JMenu();
   		HelpMenu = new JMenu();
   		GenHelpItem = new JMenuItem();
   		SettingsItem = new JMenuItem();
   		WindowItem = new JMenuItem();
   		DataItem = new JMenuItem();
    	ImportItem =  new JMenuItem();
    	CloseItem = new JMenuItem();
    	final int numButtons = 2;
    	radioButtons = new JRadioButton[numButtons];
    	group = new ButtonGroup();

    	FileMenu.setText("File");
    	SettingsMenu.setText("Settings");
    	HelpMenu.setText("Help");

		ImportItem.setText("Import");
		ImportItem.addActionListener(new java.awt.event.ActionListener() {
		public void actionPerformed(java.awt.event.ActionEvent evt) {
                ImportItemActionPerformed(evt);
		}
		});


		FileMenu.add(ImportItem);

		CloseItem.setText("Exit");
		CloseItem.addActionListener(new java.awt.event.ActionListener() {
		public void actionPerformed(java.awt.event.ActionEvent evt) {
		      CloseItemActionPerformed(evt);
		}
	    });

		SettingsMenu.add(SettingsItem);
		SettingsItem.setText("Set Training Parameters");
		SettingsItem.setEnabled(false);
		SettingsItem.addActionListener(new java.awt.event.ActionListener() {
		public void actionPerformed(java.awt.event.ActionEvent evt) {
		      SettingsItemActionPerformed(evt);
		}
	    });

	    SettingsMenu.add(DataItem);
		DataItem.setText("Select Data File");
		DataItem.addActionListener(new java.awt.event.ActionListener() {
		public void actionPerformed(java.awt.event.ActionEvent evt) {
				      DataItemActionPerformed(evt);
		}
	    });

	    SettingsMenu.add(WindowItem);
		WindowItem.setText("Set Window Size");
		WindowItem.addActionListener(new java.awt.event.ActionListener() {
		public void actionPerformed(java.awt.event.ActionEvent evt) {
						      WindowItemActionPerformed(evt);
		}
	    });


		SettingsMenu.addSeparator();

		radioButtons[1] = new JRadioButton("Train using duration");
		radioButtons[1].setActionCommand(durButton);
		radioButtons[1].setSelected(true);
		SettingsMenu.add(radioButtons[1]);
	 	radioButtons[1].addActionListener(new java.awt.event.ActionListener() {
				public void actionPerformed(java.awt.event.ActionEvent evt) {
				      durationActionPerformed(evt);
				}
	    });

		radioButtons[0] = new JRadioButton("Train using pitch");
		radioButtons[0].setActionCommand(pitButton);
	    SettingsMenu.add(radioButtons[0]);
		radioButtons[0].addActionListener(new java.awt.event.ActionListener() {
				public void actionPerformed(java.awt.event.ActionEvent evt) {
					   pitchActionPerformed(evt);
				}
	    });

		HelpMenu.add(GenHelpItem);
		GenHelpItem.setText("Basic Operation");
		GenHelpItem.addActionListener(new java.awt.event.ActionListener() {
				public void actionPerformed(java.awt.event.ActionEvent evt) {
						      GenHelpItemActionPerformed(evt);
				}
	    });


	    FileMenu.add(CloseItem);
		TitleBar.add(FileMenu);
		TitleBar.add(SettingsMenu);
		TitleBar.add(HelpMenu);
		setJMenuBar(TitleBar);

	}

	private void initToolBar() {

		toolBar = new JToolBar();


		toolBar.setRollover(true);

		importButton=new JButton(new ImageIcon("images/general/Import16.gif"));
		processButton = new JButton(new ImageIcon("images/development/Host16.gif"));
		playButton=new JButton(new ImageIcon("images/media/Play16.gif"));
		stopButton=new JButton(new ImageIcon("images/media/Stop16.gif"));
		removeButton = new JButton(new ImageIcon("images/general/Remove16.gif"));

		toolBar.add(importButton);
		importButton.setToolTipText("Import MIDI files");
		importButton.addActionListener(new java.awt.event.ActionListener() {
		public void actionPerformed(java.awt.event.ActionEvent evt) {
                ImportItemActionPerformed(evt);
		}
		});

		toolBar.add(processButton);
		processButton.setEnabled(false);
		processButton.setToolTipText("Compose");
		processButton.addActionListener(new java.awt.event.ActionListener() {
		public void actionPerformed(java.awt.event.ActionEvent evt) {
                processButtonActionPerformed(evt);
		}
		});
		toolBar.addSeparator();

		toolBar.add(removeButton);
		removeButton.setEnabled(false);
		removeButton.setToolTipText("Removes selected MIDI file from list");
		removeButton.addActionListener(new java.awt.event.ActionListener() {
		public void actionPerformed(java.awt.event.ActionEvent evt) {
		         removeButtonActionPerformed(evt);
		}
		});



		toolBar.add(playButton);
		playButton.setEnabled(false);
		playButton.setToolTipText("Play selected MIDI file");
		playButton.addActionListener(new java.awt.event.ActionListener() {
		public void actionPerformed(java.awt.event.ActionEvent evt) {
		         playButtonActionPerformed(evt);
		}
		});

		toolBar.add(stopButton);
		stopButton.setEnabled(false);
		stopButton.setToolTipText("Stop");
		stopButton.addActionListener(new java.awt.event.ActionListener() {
		public void actionPerformed(java.awt.event.ActionEvent evt) {
				stopButtonActionPerformed(evt);
		}
		});

		getContentPane().add(toolBar, BorderLayout.NORTH);

	}


	private void initMidiPane() {

		listModel = new DefaultListModel();
		list = new JList(listModel);
		list.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);

		midiPane = new JScrollPane(list);
		TitledBorder tb1 = new TitledBorder("MIDI Input Files");
		midiPane.setBorder(tb1);

		listModel1 = new DefaultListModel();
		outputList = new JList(listModel1);
		outputList.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);

		list.addFocusListener(this);
		outputList.addFocusListener(this);

		mainPane = new JScrollPane(outputList);
		TitledBorder tb = new TitledBorder("MIDI Output File");
		mainPane.setBorder(tb);

		splitPane = new JSplitPane(JSplitPane.VERTICAL_SPLIT,
						                                   midiPane, mainPane);
		splitPane.setOneTouchExpandable(true);
		splitPane.setDividerLocation(200);

		Dimension minimumSize = new Dimension(100, 50);
		midiPane.setMinimumSize(minimumSize);
		mainPane.setMinimumSize(minimumSize);

		splitPane.setPreferredSize(new Dimension(400,200));
        getContentPane().add(splitPane);


	}

	private void initStatus() {
		statusField = new JTextField("Click 'Import' to import MIDI files");
		statusField.setEditable(false);
		getContentPane().add(statusField, BorderLayout.SOUTH);

	}

	/** Event Handler **/

    /** Action listener menthods **/




 	private void ImportItemActionPerformed(java.awt.event.ActionEvent evt) {
        Object target=evt.getSource();
        if(target == ImportItem || target== importButton) {
			statusField.setText("Use 'Ctrl + A' to select all");
			processFiles();
			if(fileOpen) {
				processButton.setEnabled(true);
				playButton.setEnabled(true);
				SettingsItem.setEnabled(true);
				removeButton.setEnabled(true);
			}
		}
    }


    private void CloseItemActionPerformed(java.awt.event.ActionEvent evt) {
 		Object target=evt.getSource();
        	if(target== CloseItem) {
				confirmExit();
        }
    }

   public void SettingsItemActionPerformed(java.awt.event.ActionEvent evt) {
           Object target=evt.getSource();
           int y;
           if(target== SettingsItem) {
			for (y=0;y < files.length;y++);
			new bparsFrame(y);

			}
    }

 	private void DataItemActionPerformed(java.awt.event.ActionEvent evt) {
           Object target=evt.getSource();
           if(target== DataItem) {
		   processDataInput();

		   }
    }

	private void WindowItemActionPerformed(java.awt.event.ActionEvent evt) {
	        Object target=evt.getSource();
	        if(target== WindowItem) {
			   ws= new winSizeFrame(winSize);
			   winSet=true;
			}
	}


	private void pitchActionPerformed(java.awt.event.ActionEvent evt) {
           Object target=evt.getSource();
           if(target== radioButtons[0]) {
			radioButtons[0].setSelected(true);
			radioButtons[1].setSelected(false);
			pitch=true;
		   }
    }


	private void durationActionPerformed(java.awt.event.ActionEvent evt) {
           Object target=evt.getSource();
		    if(target== radioButtons[1]) {
		   	radioButtons[1].setSelected(true);
		   	radioButtons[0].setSelected(false);
		   	pitch=false;
		   }
    }

	private void GenHelpItemActionPerformed(java.awt.event.ActionEvent evt) {
           Object target=evt.getSource();
           int general=0;
		    if(target== GenHelpItem) {
		    new HelpFrame(general);
		   }
    }


	private void removeButtonActionPerformed(java.awt.event.ActionEvent evt) {
           Object target=evt.getSource();
           int toRemove,outRemove;
           String toDelete;
           statusField.setText("");
           if(target== removeButton) {
					if(!list.isSelectionEmpty()) {
					toRemove= list.getSelectedIndex();
					listModel.removeElementAt(toRemove);

					}
					else if(!outputList.isSelectionEmpty()) {
					outRemove=outputList.getSelectedIndex();
					toDelete =(String)outputList.getModel().getElementAt(outRemove);
					deleteFiles(toDelete);
					listModel1.removeElementAt(outRemove);

					}

		   }
    }

    private void playButtonActionPerformed(java.awt.event.ActionEvent evt) {
		     Object target=evt.getSource();
		     if(target == playButton) {
			 	stopButton.setEnabled(true);
			 	temp = null;
					if (!list.isSelectionEmpty()) {
						listActive=false;
						temp = list;
					}
					else if (!outputList.isSelectionEmpty()) {
						temp = outputList;
						listActive=true;
					}
					if (temp != null) {
						playAudio();

					} else {
					statusField.setText("No files selected");
					}
			}



	}

	private void stopButtonActionPerformed(java.awt.event.ActionEvent evt) {
	        Object target=evt.getSource();
	        if(target == stopButton) {
				currentClip.stop();
				playButton.setEnabled(true);
				stopButton.setEnabled(false);
				temp.clearSelection();
				statusField.setText(" ");
			}
    }

	public void focusGained(FocusEvent e) {

		Component lost = e.getOppositeComponent();
		if(lost instanceof JList) {
			((JList)lost).clearSelection();
		}
	}

	public void focusLost(FocusEvent e) {
		if (e.getComponent() instanceof JButton) {
			list.clearSelection();
			outputList.clearSelection();
		}
	}

	private void processButtonActionPerformed(java.awt.event.ActionEvent evt) {
	     Object target=evt.getSource();
	     statusField.setText("Processing...");
	     String seed = "10";
	     boolean split=true;
	     processDone=true;
	     int j=0;
	     int learn=1;
	     int reverse=5;
		 int test=3;

	     	if(target == processButton) {
				processButton.setCursor(Cursor.getPredefinedCursor(Cursor.WAIT_CURSOR));

				if(winSet) {
					winSize=ws.getWinSize();
				}

				appendFiles(fileArray,txtArray,".txt");
				appendFiles(txtArray,numArray,".num");
				appendFiles(numArray,pitchArray,".pit");
				appendFiles(numArray,durArray,".dur");
				setPaths();


				for(j=0;fileArray[j] !=null;j++){
					// Use MF2T to convert midi files to text
					mf2t(fileArray[j],txtArray[j]);
					// Use T2N to reformat the text files into normalised numerical files
					t2n(txtArray[j],numArray[j]);
					// Split pitch and duration into separate files
					fsplit(numArray[j],pitchArray[j],durArray[j],split);
				}

				int lineTotal=j;
					if(pitch) {
						// all pitch files copied into one allpitch file
						setOutput("allPitch.pit", j);

						for(j=0;pitchArray[j] !=null;j++) {
							tuneNumbers(lineTotal,j);
							allPitchDur(pitchArray,j);
						}

						closeOutput();

						String pitchF = path + "allPitch.pit";
						bp(learn,pitchF,wtsFile,outFile,hidFile,seed,path);
					}

					else if(!pitch) {


					// all duration files copied into one alldur file
					setOutput("allDur.dur",j);

					for(j=0;durArray[j] !=null;j++) {
						tuneNumbers(lineTotal,j);
						allPitchDur(durArray,j);
					}
					closeOutput();

					String durF = path + "allDur.dur";
					bp(learn,durF,wtsFile,outFile,hidFile,seed,path);
				}


			String data = readFile();
			// cross
			//cross(data,crossFile);
			if(data !=null){
				bp(reverse,data,wtsFile,outFile,hidFile,seed,path);

				split=false;
				// switched order of hid File and join file
				fsplit(joinFile,hidFile,outFile,split);
				nn2t(winSize,joinFile,textOut);
				t2mf(textOut,midiOut);
				processTrack++;
				listModel1.addElement(midiOut);
				outputList.setSelectedIndex(0);
				//deleteTempFiles();

				processButton.setCursor(null);
				processButton.setEnabled(true);
				statusField.setText("Processing complete...");
				} else {
				processButton.setCursor(null);
				processButton.setEnabled(true);
				statusField.setText("Select data file: Settings -> Select Data File ");
				}

			}

	}
	/** Add tune numbers to pitchfile/duration file **/

	private void tuneNumbers(int linein,int pos) {
		int s=0;
		while(s < linein) {
			if(s==pos) {
			writeOutput("1");
			writeOutput(" ");
			}
		writeOutput("0");
		writeOutput(" ");
		s++;
		}
	}

	/** write to pitch/duration file **/

	private void writeOutput(String input) {
		try {
		out.write(input);
		}
		catch(IOException coe) {
			processError(tuneWrite);
		}
	}

	/** setup pitch/duration files for reading and writing **/

	private void setOutput(String outF, int lines) {
		outputFile = new File(outF);
		try {
		out = new FileWriter(path + outputFile);
		}
		catch(IOException ioe) {
			processError(fileWrite);
		}
		try {
		out.write(Integer.toString(lines) + " 0" + "\r\n");
		}
		catch(IOException coe) {
			processError(fileNumber);
		}


	}

	/** close output pitch/duration file **/
	private void closeOutput() {
		try {
		out.close();
		}
		catch(IOException coe) {
				processError(fileClose);
		}
	}

	/** Check that the raw data file is present before use **/
	private void checkData(String dataName) {
		File rawdata = new File(dataName);

		try {
		streamin = new FileReader(rawdata);
		}
		catch(FileNotFoundException fn) {
				processError(rawRead);
		}
		try {
			streamin.close();
		}
		catch(IOException roe) {
				processError(rawWrite);
		}


	}

	/** Copy all files into one pitch/duration file **/

	private void allPitchDur(String[] infile, int pos) {
		File inputFile = new File(infile[pos]);

		try {
			in = new FileReader(inputFile);
		}
		catch(FileNotFoundException fn) {
			processError(fileRead);
		}
		try {
			int c;
			while((c = in.read()) !=-1) {
			out.write(c);
			}
			in.close();
		}
		catch(IOException woe) {
				processError(fileWrite);
		}

	}

	/** Import File processing **/

	private void  processFiles() {
		String[] text = new String[] { "txt"};
		String[] audio = new String[] { "mid"};
		JFileChooser chooser = new JFileChooser();
		chooser.setMultiSelectionEnabled(true);

		chooser.addChoosableFileFilter(new CustomFileFilter(text,
										  "Text Files (*.txt)"));
		chooser.addChoosableFileFilter(new CustomFileFilter(audio,
										  "MIDI Files (*.mid)"));

		chooser.setDialogType(JFileChooser.CUSTOM_DIALOG);
		chooser.setApproveButtonText("Import");

		int option = chooser.showDialog(this,"Import");

		if(option == JFileChooser.APPROVE_OPTION) {
			fileOpen=true;
			fileCount++;
			if(fileCount > 1){
				listModel.clear();
			}
			parseInputFiles(chooser);

		}


	}

	/** Create arrays for use during processing **/

	private void parseInputFiles(JFileChooser chooser) {
		files = chooser.getSelectedFiles();
	 	int i,l;

	 	for (l=0;l < files.length;l++);
	 	fileArray = new String[l+1];
	 	listArray = new String[l+1];
		txtArray= new String[l+1];
		numArray = new String[l+1];
		pitchArray = new String[l+1];
		durArray = new String[l+1];
    	fileBuf= new StringBuffer[l+1];
		setBpars(l);

		for (i = 0; i < files.length; i++) {
			 listArray[i]=files[i].getName();
			 fileArray[i]=files[i].getAbsolutePath();
			 listModel.addElement(listArray[i]);
       	}
		statusField.setText(i + " MIDI file(s)");
	}

	/** Add file extensions **/

	private void appendFiles(String[] in,String[] out,String append) {
	int p=0;
		while(in[p] !=null) {
		 int lent=in[p].length();
		 fileBuf[p]= new StringBuffer(lent);

			for (int n=0; n<=lent&& in[p].charAt(n) !='.'; n++) {
				fileBuf[p].append(in[p].charAt(n));
			}
		out[p]=fileBuf[p] + append;
		p++;
		}
	}

	/** Add full path names to output files **/

	private void setPaths() {
 		path = fileArray[0].substring(0, fileArray[0].lastIndexOf("\\") + 1);
		wtsFile = path + "wtsFile.wts";
		textOut = path + "txtOut.txt";
		midiOut = path + processTrack + "midiOut.mid";
		outFile = path + "outFile.out";
		hidFile = path + "hidFile.hid";
		joinFile= path + "joinFile.num";
		crossFile=path + "crossFile.cro";
		logPath = path + "logFile.log";

	}

	/** Show data file selection dialog **/

	private void processDataInput() {
		JFileChooser dataChooser = new JFileChooser();
		dataChooser.setDialogType(JFileChooser.CUSTOM_DIALOG);
		dataChooser.setApproveButtonText("Select");
	    int choice = dataChooser.showDialog(this,"Select");

		if(choice == JFileChooser.APPROVE_OPTION) {
			dataFile =  dataChooser.getSelectedFile();
			dataPath = dataFile.getAbsolutePath();
			savePath(dataPath);
		}
	}

	/** Save path to data file **/

	private void savePath(String toSave) {
		pathFile = new File("settings.ini.");
		try {
		outData = new FileWriter(pathFile);
		}
		catch(IOException ioe) {
			processError(dataWrite);
		}
		try {
		outData.write(toSave);
		outData.close();
		}
		catch(IOException coe) {
			processError(dataWrite);
		}

	}

	/** Read settings file to find path to data file **/

	private String readFile() {
		File inputFile = new File("settings.ini");

		try {
		fis = new FileInputStream(inputFile);
		}
		catch(FileNotFoundException fn) {
			processError(fileMissing);
			return null;
		}
		try {

		byte[] contents = new byte[(int)inputFile.length()];
		fis.read(contents);
		fis.close();
		pathfromFile = new String(contents);

		fis.close();
		}
		catch ( IOException e ) {
			e.printStackTrace();
		}
	return pathfromFile;
	}


	/** Delete temporary files used during processing **/

	public void deleteTempFiles() {
		if(processDone) {
			for(int m=0;txtArray[m]!=null;m++){
				String txtDelete=txtArray[m];
				deleteFiles(txtDelete);
			}
			for(int l=0;numArray[l]!=null;l++){
				String numDelete=numArray[l];
				deleteFiles(numDelete);
			}
			for(int q=0;pitchArray[q]!=null;q++){
				String pitDelete=pitchArray[q];
				deleteFiles(pitDelete);
			}
			for(int r=0;durArray[r]!=null;r++){
				String durDelete=durArray[r];
				deleteFiles(durDelete);
			}
		deleteFiles(wtsFile);
		deleteFiles(textOut);
		deleteFiles(outFile);
		deleteFiles(hidFile);
		deleteFiles(joinFile);
		}
	}

	/** Delete method **/

	public void deleteFiles(String fileName) {
		File srcFile = new File(fileName);

		try {
			if(!srcFile.delete()) { statusField.setText("File Deletion failed");}
		}
    	catch ( SecurityException se ) {
			se.printStackTrace();
			}

	}

	/** Play MIDI files **/

	public void playAudio() {
		String toPlay;
		int selected;

		if(listActive) {
			selected= outputList.getSelectedIndex();
			toPlay =(String)outputList.getModel().getElementAt(selected);
			statusField.setText("Playing " + toPlay);
			File f = new File(toPlay);
			playFile(f);
		}
		else if (!listActive) {
			selected= list.getSelectedIndex();
			toPlay =(String)list.getModel().getElementAt(selected);
			statusField.setText("Playing " + toPlay);
			playPath = fileArray[0].substring(0, fileArray[0].lastIndexOf("\\") + 1);
			File f = new File(playPath +toPlay);
			playFile(f);
		}

		else {
			statusField.setText("No file selected");
		}
	}

	private void playFile(File mf) {
		try {
			URL u = new URL ("file://" + mf.getAbsolutePath());
		    currentClip = Applet.newAudioClip(u);
		}
		catch (Exception e) {
			e.printStackTrace();
			currentClip = null;
			statusField.setText("Error loading clip.");
			}
		currentClip.play();
		playButton.setEnabled(false);
	}

	/** Initialise BPARS file for use by back propagation net **/

	private void setBpars (int inputs) {
    	File param = new File("bpars");
    	try {
    	parout = new FileWriter(param);
    	parout.write("10000 " + "1000 " + "10 " + "0.1 " + "0.9 " +
    				  Integer.toString(inputs) + " " + "128" + " " + "20" + " "
    				  + "0.1 " + "0 " + "0 " + "1.0");
    	}
			catch(IOException ioe) {
			processError(bparError);
		}
		try {
		parout.close();
		}
		catch(IOException coe) {
			processError(bparError);
		}
	}

	/** Show exit dialog **/

	private void confirmExit() {
		boolean choice=false;
		int result=	JOptionPane.showConfirmDialog(this, "Exit the application?",
		"Confirm Exit",JOptionPane.YES_NO_CANCEL_OPTION);
			if(result ==JOptionPane.YES_OPTION){
				choice=true;
				System.exit(0);
			} else if (result == JOptionPane.NO_OPTION) {
				choice=false;
			}
	}


	/** Error mesages **/

	private void processError(String inerror) {
		JOptionPane.showMessageDialog(this,inerror);

	}

    /** Main method */

    public static void main(String args[]) {

        JDialog.setDefaultLookAndFeelDecorated(true);
        JFrame.setDefaultLookAndFeelDecorated(true);
        Toolkit.getDefaultToolkit().setDynamicLayout(true);
        System.setProperty("sun.awt.noerasebackground","true");
        new MainWindow().show();


    }

	/**  Initialise native methods **/

	public native void mf2t(String s6, String s7);
	public native void t2n(String s1,String s2);
	public native void fsplit(String s3, String s4, String s5,boolean b1);
	public native void bp(int option,String s8, String s9,String s10, String s11,String s12,String s19);
	public native void cross(String s13, String s14);
	public native void nn2t(int win,String s15, String s16);
	public native void t2mf(String s17, String s18);


	/** Load native method library **/

	static { System.loadLibrary("project3"); }


    /** Variable declarations **/

	protected int closeOp;
    private int fileCount=0;
	private int winSize=10;
	private int processTrack=0;
	public boolean processDone=false;
	public boolean fileOpen=false;
	public boolean listActive=false;
	public boolean pitch=false;
	public boolean winSet=false;
	private winSizeFrame ws;

    /** Menus **/

    private JMenuBar TitleBar;
    private JMenu FileMenu;
    private JMenu SettingsMenu;
    private JMenu HelpMenu;

    /** Menu Items **/

    private JMenuItem ImportItem;
    private JMenuItem CloseItem;
 	private JMenuItem SettingsItem;
    private JMenuItem DataItem;
    private JMenuItem WindowItem;
    private JMenuItem GenHelpItem;

	/** Toolbar **/

	private JToolBar toolBar;

	/** Lists **/

	private DefaultListModel listModel;
	private DefaultListModel listModel1;
	private JList list;
	private JList temp;
	private JList outputList;

	private JScrollPane mainPane;
	private	JScrollPane midiPane;
	private JSplitPane splitPane;
	private JTextField statusField;

	/** Buttons **/

	public ButtonGroup group;
	private JButton processButton;
	private JButton newButton;
	private JButton importButton;
	private JButton removeButton;
	private JButton playButton;
	private JButton stopButton;
	private JRadioButton[] radioButtons;
	public final String pitButton ="pitch";
	public final String durButton ="duration";

	/** File variables **/

	public File[] files;
	public File outputFile;
	public File dataFile;
	public File pathFile;
	public FileInputStream fis;
	public FileReader in;
	public FileReader streamin;
	public FileWriter out;
	public FileWriter outData;
	public FileWriter parout;
	public AudioClip currentClip;

	/** File Strings **/

	public String error;
	public String path;
	public String playPath;
	public String pathfromFile;
	public String dataPath;
	public String pitchPath;
	public String logPath;
	public String textOut;
	public String midiOut;
	public String outFile;
	public String hidFile;
	public String joinFile;
	public String wtsFile;
	public String crossFile;

	/** Arrays **/

	public String fileArray[];
	public String listArray[];
	public String txtArray[];
	public String numArray[];
	public String pitchArray[];
	public String durArray[];
	public StringBuffer fileBuf[];

	/** Error strings **/


	public String fileError = new String("Import files first");
	public String fileType = new String("Not a MIDI file");
	public String fileMissing = new String("Please select data file: Settings -> Select data file");
	public String fileRead = new String("Unable to read input file");
	public String fileWrite = new String("Unable to write output file");
	public String fileClose = new String("Unable to close output file");
	public String fileNumber = new String("Unable to write number of lines");
	public String rawRead = new String("Unable to read from raw data file");
	public String rawWrite = new String("Unable to close raw data file");
	public String tuneWrite = new String("Unable to write tune numbers");
	public String bparError = new String("Unable to write to bpars");
	public String dataWrite = new String("Unable to save path to data file");

    // End of variable declaration

}

