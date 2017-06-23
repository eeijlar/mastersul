import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.io.File;
import java.io.*;
import javax.swing.event.*;

public class bparsFrame extends JFrame {
	public JButton save;
	public JButton defaultButton;
	public FileWriter bpout;
	public JTextField t1;
	public JTextField t2;
	public JTextField t3;
	public JTextField t4;
	public JTextField t5;
	public JTextField t6;
	public JTextField t7;
	public JTextField t8;
	public JTextField t9;
	public JTextField t10;
	public JTextField t11;
	public JTextField t12;
	public JTextField t13;
	//public String cycle="2000";
	public int nofiles;
	public int wSize=10;


	public bparsFrame(int noOfFiles)
	{
		super();
		nofiles=noOfFiles;
		getContentPane().setLayout(new GridLayout(8, 0)) ;

		initPanel();
		setDefault();
		pack();
		setVisible(true);

	}

	public void initPanel()
	{
	//10000 1000 10 0.1 0.9 52 128 20 0.1 0 0 1.0

		JPanel p1=new JPanel();

		t1= new JTextField(5);
		JLabel j1= new JLabel("Timeout Cycles ");
		p1.add(j1);
		p1.add(t1);
		t2= new JTextField(5);
		JLabel j2= new JLabel("Display Cycles ");
		p1.add(j2);
		p1.add(t2);
		getContentPane().add(p1);

		JPanel p3=new JPanel();

		t3= new JTextField(5);
		JLabel j3= new JLabel("Learning Cycles");
		p3.add(j3);
		p3.add(t3);
		t4= new JTextField(5);
		JLabel j4= new JLabel("Learning Rate  ");
		p3.add(j4);
		p3.add(t4);
		getContentPane().add(p3);

		JPanel p5=new JPanel();

		t5= new JTextField(5);
		JLabel j5= new JLabel("Momentum      ");
		p5.add(j5);
		p5.add(t5);
		t6= new JTextField(5);
		t6.setEditable(false);
		JLabel j6= new JLabel("No. of Inputs  ");
		p5.add(j6);
		p5.add(t6);
		getContentPane().add(p5);


		JPanel p7=new JPanel();

		t7= new JTextField(5);
		JLabel j7= new JLabel("No. of Outputs");
		p7.add(j7);
		p7.add(t7);
		JPanel p8=new JPanel();
		t8= new JTextField(5);
		JLabel j8= new JLabel("No. of Hidden ");
		p7.add(j8);
		p7.add(t8);
		getContentPane().add(p7);


		JPanel p9=new JPanel();

		t9= new JTextField(5);
		JLabel j9= new JLabel("Tolerance      ");
		p9.add(j9);
		p9.add(t9);
		t10= new JTextField(5);
		JLabel j10= new JLabel("Label Inputs  ");
		p9.add(j10);
		p9.add(t10);
		getContentPane().add(p9);

		JPanel p11=new JPanel();
		t11= new JTextField(5);
		JLabel j11= new JLabel("Criteria       ");
		p11.add(j11);
		p11.add(t11);
		t12= new JTextField(5);
		JLabel j12= new JLabel("Percentage    ");
		p11.add(j12);
		p11.add(t12);
		getContentPane().add(p11);

		save = new JButton("Apply");
		save.addActionListener(new java.awt.event.ActionListener() {
				public void actionPerformed(java.awt.event.ActionEvent evt) {
		                saveActionPerformed(evt);
				}
		});

		JPanel p14 = new JPanel();
		p14.add(save);
		getContentPane().add(p14);

	}
	private void saveActionPerformed(java.awt.event.ActionEvent evt) {
	           Object target=evt.getSource();
	           if(target== save) {
				writeBpars();
				save.setEnabled(false);
				}
    }

    private void writeBpars() {
    	File par = new File("bpars");
    	try {
    	bpout = new FileWriter(par);
    	bpout.write(t1.getText() + " " + t2.getText()
    	+ " " + t3.getText()+ " " + t4.getText()
    	+ " " + t5.getText()+ " " + Integer.toString(nofiles)
    	+ " " + t7.getText()+ " " + t8.getText()
    	+ " " + t9.getText()+ " " + t10.getText()
    	+ " " + t11.getText()+ " " + t12.getText()
    	);
    	}
			catch(IOException ioe) {
		System.out.println("Error writing to bpars");
		}
		try {
		bpout.close();
		}
		catch(IOException coe) {

		}
	}

//"Cycles","cyclesDisp","cycleTest","Rate","momentum","inputs",
//"outputs","hidden","tolerance","labelInput","Criteria","percentage";

	private void setDefault() {
		t1.setText("10000");
		t2.setText("10");
		t3.setText("10");
		t4.setText("0.1");
		t5.setText("0.9");
		t6.setText(Integer.toString(nofiles));
		t7.setText("128");
		t8.setText("20");
		t9.setText("0.1");
		t10.setText("0");
		t11.setText("0");
		t12.setText("1.0");
	}


/*
	public static void main(String args[]) {
		int filess=10;
		bparsFrame bp= new bparsFrame(filess);
	}

*/
}

