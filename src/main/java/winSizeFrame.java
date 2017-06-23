import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.io.File;
import java.io.*;
import javax.swing.event.*;

public class winSizeFrame extends JFrame {
	JTextField t13;
	int wSize;
	JButton c;


	public winSizeFrame(int size) {

	super();
	wSize=size;
	getContentPane().setLayout(new GridLayout(2,0));
	JPanel p13=new JPanel();
	t13= new JTextField(5);
	JLabel j13= new JLabel("Current Window Size");
	p13.add(j13);
	p13.add(t13);

	JPanel pp =new JPanel();
	c = new JButton("Apply");
	c.addActionListener(new java.awt.event.ActionListener() {
		public void actionPerformed(java.awt.event.ActionEvent evt) {
			cActionPerformed(evt);
	}
	});
	pp.add(c);

	getContentPane().add(p13);
	getContentPane().add(pp);
	t13.setText(Integer.toString(wSize));
	pack();
	setVisible(true);
	}


	private void cActionPerformed(java.awt.event.ActionEvent evt) {
	           Object target=evt.getSource();
	           if(target== c) {
				wSize=Integer.parseInt(t13.getText());
				c.setEnabled(false);

				}
   }

	public int getWinSize() {
		return wSize;
	}


}