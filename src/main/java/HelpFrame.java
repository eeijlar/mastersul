import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.io.File;
import java.io.*;
import javax.swing.event.*;

public class HelpFrame extends JFrame {

	JButton dismiss;
	JTextArea textArea;
	FileReader inhlp;

	public HelpFrame(int option)
	{
		super();
		initFrame();
		readFile();
		pack();
		setSize(600,400);
		setVisible(true);
	}

	public void initFrame()
	{	JPanel p1= new JPanel();
		JPanel p2= new JPanel();
		p1.setLayout(new BorderLayout ());
		textArea = new JTextArea();
		textArea.setEditable(false);
		Font boldFont = new Font("Helvetica", Font.BOLD, 12);
		textArea.setFont(boldFont);
		dismiss = new JButton("OK");
		textArea.setLineWrap(true);
		JScrollPane mainPane = new JScrollPane(textArea);
		p1.add(mainPane);
		p2.add(dismiss);
		getContentPane().add(p1);
		getContentPane().add(p2,BorderLayout.SOUTH);

		dismiss.addActionListener(new java.awt.event.ActionListener() {
				public void actionPerformed(java.awt.event.ActionEvent evt) {
		                dismissActionPerformed(evt);
				}
		});



	}

	 private void dismissActionPerformed(java.awt.event.ActionEvent evt) {
	           Object target=evt.getSource();
	           if(target== dismiss) {
				this.setVisible(false);
				}
    }


	private void readFile() {
			File inputFile = new File("genUse.txt");
			try {
		    inhlp = new FileReader(inputFile);
			}
			catch(FileNotFoundException fn) {
				JOptionPane.showMessageDialog(this,"Help file missing");
			}
			try {
			textArea.read(inhlp, null);
			inhlp.close();
			}
			catch ( IOException e ) {
					e.printStackTrace();
			}
	}

}