import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;
public class NoTippingGame
{
	
	public static void main(String[] args)
	{
		try
		{
			int portNum=Integer.parseInt(args[0]);
			int weightNum=Integer.parseInt(args[1]);
			boolean red=args[2].equalsIgnoreCase("red");
			NoTippingGame ntg=new NoTippingGame(portNum, weightNum, red);
        	
			ntg.run(state);
		}catch(Exception e)
		{
			e.printStackTrace();
		}
		
		/*String str="";
		File wf;
		FileWriter wr;
		try
		{
			wf=new File("noTippingOutput.txt");
			wr=new FileWriter(wf);
			
		}catch(Exception e)
		{
			e.printStackTrace();
		}*/
	}
		//precomputation stuff
		//set1[i][0]=left most position where weight i+1 alone would be stable
		//set1[i][1]=right most position where weight i+1 alone would be stable
	public int[][] set1={{-10,-4},{-7,-3},{-6,-2},{-5,-2},{-4,-2},
						{-4,-2},{-4,-2},{-4,-2},{-3,-2},{-3,-2}	};
						
	//actual storage stuff
	public boolean[] inSetOne=new boolean[21];//position on set1
	public boolean[] notInSetOne=new boolean[21];//other position
	public int weightOnLever=0;//numOfWeights on Lever
	public int[] positions= new int[21];//[pos+10]holds weight number
	public int[][] torqueCon= new int[21][2];//holds indiviual torque contribution
										//for both supports
	public int[] redW;//location of the weights, 
	public int[] blueW;//
		//remember there is an implicit weight of 3 at pos 0
	public final int[] supports={-3,-1};
		//torque at -3 must be negative and torque at -1 must be positive
	public int[] torque;//[2]
		//torques of the supports with initial configuration , w
		//to calculate new torques, find the first difference in the 
		//game state passed to here, add accordingly
	//removal phase
	public int[][] treeStorage;
	public final int TIPPED=-500;
	public int[] mask={1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384};
	public int[] positionAndMask=new int[21];
	public boolean firstRemove=true;
	public final int NOT_ON_LEVER=-50;
	public boolean addingPhase;//true, adding, false removing
		//store player info
	public boolean playingAs;
	public final boolean RED=true;
	public final boolean BLUE=false;
	public int port;
	public int weightNum;
	
	
	public NoTippingGame(int portNum, int mw, boolean pl)
	{
		torque=new int[2];
		torque[0]=-9;
		torque[1]=-3;
		redW=new int[mw];
		blueW=new int[mw];
		for( int i=0;i<mw;i++)
		{
			redW[i]=NOT_ON_LEVER;
			blueW[i]=NOT_ON_LEVER;
		}
		playingAs=pl;
		port=portNum;
		weightNum=mw;
		treeStorage=new int[32768][2];
		//run();
	}
	
	public String run(String state)
	{		
		try
		{
			BufferedReader in = null;        	
            in = new BufferedReader(System.in);
        	String command="";
			while ((command = in.readLine())!= null) {
                if (command.equals("STATE END")) {
                    System.out.println(process(state.toString()));
                    state.delete(0, state.length());
                    continue;
                }
                state.append(command+"\n");
            }
        }catch(Exception e)
        {
        	e.printStackTrace();
        }
        
	}
	
	public void run()
	{
		ServerSocket server = null;
        Socket socket = null;
        PrintWriter out = null;
        BufferedReader in = null;
        try {
            server = new ServerSocket(port);
            socket = server.accept();
            socket.setTcpNoDelay(true);
            out = new PrintWriter(socket.getOutputStream(), true);
            in = new BufferedReader(new InputStreamReader(socket.getInputStream()));

        } catch (IOException ev) {
            System.err.println(ev.getMessage());
        }
        String command;
        StringBuffer state = new StringBuffer();
        try {
            while ((command = in.readLine())!= null) {
                if (command.equals("STATE END")) {
                    out.println(process(state.toString()));
                    state.delete(0, state.length());
                    continue;
                }
                state.append(command+"\n");
            }
        }
        catch (IOException io) {
            System.err.println(io.getMessage());
        }
        out.close();
        try {
            in.close();
            socket.close();
            server.close();
        } catch (IOException io) {
            System.err.println(io.getMessage());
        }
	}
	
	public String process(String state)
	{
		String[] states=state.split("\n");
		if(states[0].equals("ADDING"))
		{
			addingPhase=true;
		}else
		{
			addingPhase=false;
		}		
		for(int i=1;i<states.length;i++)
		{
			String[] aLine=states[i].split(" ");
			try
			{
				int w=Integer.parseInt(aLine[3]);
				int p=Integer.parseInt(aLine[1]);
				if(aLine[0].equals("1")&&positions[p+10]!=w)
				{		
//System.out.print(" S1:"+p+","+w+" ");			
					weightOnLever++;
					positions[p+10]=w;
					torqueCon[p+10][0]=(p-supports[0])*-w;
					torqueCon[p+10][1]=(p-supports[1])*-w;
					torque[0]+=torqueCon[p+10][0];
					torque[1]+=torqueCon[p+10][1];
					if(isInSetOne(p,w))
					{
//System.out.print(" ONE1:"+p+","+w+" ");
						inSetOne[p+10]=true;
					}else
					{
//System.out.print(" NOTONE1:"+p+","+w+" ");
						notInSetOne[p+10]=true;
					}
					if(aLine[2].equals("Red"))
					{
//System.out.print(" Red1:"+p+","+w+" ");
						redW[w-1]=p;
					}else if(aLine[2].equals("Blue"))
					{
//System.out.print(" Blue1:"+p+","+w+" ");
						blueW[w-1]=p;
					}
				}else if(aLine[0].equals("2"))
				{		
//System.out.print(" S2:"+p+","+w+" ");
					if(aLine[2].equals("Red"))
					{
//System.out.print(" Red2:"+p+","+w+" ");
						p=redW[w-1];
						redW[w-1]=NOT_ON_LEVER;
					}else if(aLine[2].equals("Blue"))
					{
//System.out.print(" Blue2:"+p+","+w+" ");
						p=blueW[w-1];
						blueW[w-1]=NOT_ON_LEVER;
					}else if(aLine[2].equals("Green"))
					{
						p=-4;
					}		
//System.out.print(" S22:"+p+","+w+" ");
					if(p!=NOT_ON_LEVER)
					{							
						weightOnLever--;
						torque[0]-=torqueCon[p+10][0];
						torque[1]-=torqueCon[p+10][1];
						torqueCon[p+10][0]=0;
						torqueCon[p+10][1]=0;
						positions[p+10]=0;
						inSetOne[p+10]=false;
						notInSetOne[p+10]=false;
					}
				}
			}catch(Exception e)
			{
				e.printStackTrace();
			}
		}
		if(addingPhase)
		{
			if(playingAs==RED)
			{
				return addRed();
			}else
			{
				return addBlue();
			}
		}else
		{
			if(weightOnLever<=15)
			{
				if(firstRemove)
				{
					int j=0;
					for(int i=0;i<21;i++)
					{
						if(positions[i]!=0)
						{
							positionAndMask[i]=j;
							j++;
						}
					}
					for(int i=0;i<21;i++)
					{
System.out.print(" ["+torqueCon[i][0]+","+torqueCon[i][1]+"]");
					}
System.out.print(" TOTAL:["+torque[0]+","+torque[1]+"]");
					firstRemove=false;
				}
				if(playingAs==RED)
				{
					return removeRed();
				}else
				{
					return removeBlue();
				}
			}
			if(playingAs==RED)
			{
				return simpleRemoveRed();
			}else
			{
				return simpleRemoveBlue();
			}
		}
	}
	
	public String removeRed()
	{
System.out.print(" rm red ");
		int bitMask=0;
		for(int i=0;i<21;i++)	
		{
			if(positions[i]!=0)
			{
				bitMask|=mask[positionAndMask[i]];
System.out.print(" "+(i-10)+" ");
			}
		}
//System.out.print(" ("+Integer.toBinaryString(bitMask)+") ");
//if it is zero, run minimax
		if(treeStorage[bitMask][0]!=0)//hopefully is one , else lose for certain
		{
			if(treeStorage[bitMask][0]==-1)
			{
				System.out.println("!!!");
			}
			int w=positions[treeStorage[bitMask][1]+10];
			weightOnLever--;
			notInSetOne[treeStorage[bitMask][1]+10]=false;
			inSetOne[treeStorage[bitMask][1]+10]=false;
			torque[0]-=torqueCon[treeStorage[bitMask][1]+10][0];
			torque[1]-=torqueCon[treeStorage[bitMask][1]+10][1];
			torqueCon[treeStorage[bitMask][1]+10][0]=0;
			torqueCon[treeStorage[bitMask][1]+10][1]=1;
			positions[treeStorage[bitMask][1]+10]=0;
			return (treeStorage[bitMask][1])+" "+w;
		}
		recursiveRmRed(bitMask, copy(positions), weightOnLever,copy(torque),true);
		if(treeStorage[bitMask][0]!=0)//hopefully is one , else lose for certain
		{
			if(treeStorage[bitMask][0]<0)
			{
				System.out.println("!!!");
			}
			int w=positions[treeStorage[bitMask][1]+10];
			weightOnLever--;
			notInSetOne[treeStorage[bitMask][1]+10]=false;
			inSetOne[treeStorage[bitMask][1]+10]=false;
			torque[0]-=torqueCon[treeStorage[bitMask][1]+10][0];
			torque[1]-=torqueCon[treeStorage[bitMask][1]+10][1];
			torqueCon[treeStorage[bitMask][1]+10][0]=0;
			torqueCon[treeStorage[bitMask][1]+10][1]=1;
			positions[treeStorage[bitMask][1]+10]=0;
			return (treeStorage[bitMask][1])+" "+w;
		}
		return "error rm red";
	}
	
	public void recursiveRmRed(int bm,int[] pos, int wLeft, int[] t, boolean max)
	{
		if(t[0]>0||t[1]<0)
		{
			treeStorage[bm][0]=TIPPED;
			return;
//System.out.print(" ("+Integer.toBinaryString(bm)+"TIPPED) ");
		}
		if(wLeft==1)
		{
			treeStorage[bm][0]=-1;
System.out.print(" (LOST"+Integer.toBinaryString(bm)+","+t[0]+","+t[1]+") ");
			for(int i=0;i<21;i++)
			{
				if(pos[i]!=0)
				{
					treeStorage[bm][1]=i-10;
					return;
				}
			}
		}
		if(max)
		{
			int maxNum=-1500;
			int move=-50;
			//int maxTorque=0;
			for(int i=0;i<21;i++)//i is pos
			{//red max remove set1
				if(pos[i]!=0&&inSetOne[i])
				{
					int bitMask=bm;
					bitMask^=mask[positionAndMask[i]];
					if(treeStorage[bitMask][0]==0)
					{
						int w=pos[i];
						pos[i]=0;
						t[0]-=torqueCon[i][0];
						t[1]-=torqueCon[i][1];
						recursiveRmRed(bitMask,pos,wLeft-1,t,!max);
						pos[i]=w;
						t[0]+=torqueCon[i][0];
						t[1]+=torqueCon[i][1];
					}
					if(treeStorage[bitMask][0]==1)
					{
						treeStorage[bm][0]=1;
						treeStorage[bm][1]=i-10;
						return;
					}else if(treeStorage[bitMask][0]==-1)
					{
						maxNum=-1;
						move=i-10;
					}else if(treeStorage[bitMask][0]==TIPPED)
					{
						maxNum=-1;
						if(move<-10)
						{
							move=i-10;
						}
					}
				}
			}
			for(int i=20;i>=0;i--)//i is pos
			{//red max remove right side, so set1 can be remove later
				if(pos[i]!=0)
				{
					int bitMask=bm;
					bitMask^=mask[positionAndMask[i]];
					if(treeStorage[bitMask][0]==0)
					{
						int w=pos[i];
						pos[i]=0;
						t[0]-=torqueCon[i][0];
						t[1]-=torqueCon[i][1];
						recursiveRmRed(bitMask,pos,wLeft-1,t,!max);
						pos[i]=w;
						t[0]+=torqueCon[i][0];
						t[1]+=torqueCon[i][1];
					}
					if(treeStorage[bitMask][0]==1)
					{
						treeStorage[bm][0]=1;
						treeStorage[bm][1]=i-10;
						return;
					}else if(treeStorage[bitMask][0]==-1)
					{
						maxNum=-1;
						move=i-10;
					}else if(treeStorage[bitMask][0]==TIPPED)
					{
						maxNum=-1;
						if(move<-10)
						{
							move=i-10;
						}
					}
				}
			}
			treeStorage[bm][0]=maxNum;
			treeStorage[bm][1]=move;
			return;
		}else
		{
			int maxNum=5;
			int move=-50;
			for(int i=0;i<21;i++)
			{//red min remove left side not in set1,so red would have hard time getting set1
				if(pos[i]!=0&&notInSetOne[i])
				{
					int bitMask=bm;
					bitMask^=mask[positionAndMask[i]];
					if(treeStorage[bitMask][0]==0)
					{
						int w=pos[i];
						pos[i]=0;
						t[0]-=torqueCon[i][0];
						t[1]-=torqueCon[i][1];
						recursiveRmRed(bitMask,pos,wLeft-1,t,!max);
						pos[i]=w;
						t[0]+=torqueCon[i][0];
						t[1]+=torqueCon[i][1];
					}
					if(treeStorage[bitMask][0]==-1)
					{
						treeStorage[bm][0]=-1;
						treeStorage[bm][1]=i-10;
						return;
					}else if(treeStorage[bitMask][0]==1)
					{
						maxNum=1;
						move=i-10;
					}else if(treeStorage[bitMask][0]==TIPPED)
					{
						maxNum=1;
						if(move<-10)
						{
							move=i-10;
						}
					}
				}
			}
			for(int i=20;i>=0;i--)
			{
				if(pos[i]!=0)
				{//red min remove, remove anything that works
					int bitMask=bm;
					bitMask^=mask[positionAndMask[i]];
					if(treeStorage[bitMask][0]==0)
					{
						int w=pos[i];
						pos[i]=0;
						t[0]-=torqueCon[i][0];
						t[1]-=torqueCon[i][1];
						recursiveRmRed(bitMask,pos,wLeft-1,t,!max);
						pos[i]=w;
						t[0]+=torqueCon[i][0];
						t[1]+=torqueCon[i][1];
					}
					if(treeStorage[bitMask][0]==-1)
					{
						treeStorage[bm][0]=-1;
						treeStorage[bm][1]=i-10;
						return;
					}else if(treeStorage[bitMask][0]==1)
					{
						maxNum=1;
						move=i-10;
					}else if(treeStorage[bitMask][0]==TIPPED)
					{
						maxNum=1;
						if(move<-10)
						{
							move=i-10;
						}
					}
				}
			}
			treeStorage[bm][0]=maxNum;
			treeStorage[bm][1]=move;
			return;
		}
	}
	
	public String removeBlue()
	{
System.out.print(" rm blue ");
		int bitMask=0;
		for(int i=0;i<21;i++)	
		{
			if(positions[i]!=0)
			{
				bitMask|=mask[positionAndMask[i]];
System.out.print(" "+(i-10)+" ");
			}
		}
//System.out.print(" ("+Integer.toBinaryString(bitMask)+") ");
		if(treeStorage[bitMask][0]!=0)//hopefully is one , else lose for certain
		{
			if(treeStorage[bitMask][0]==-1)
			{
				System.out.println("!!!");
			}
			int w=positions[treeStorage[bitMask][1]+10];
			weightOnLever--;
			notInSetOne[treeStorage[bitMask][1]+10]=false;
			inSetOne[treeStorage[bitMask][1]+10]=false;
			torque[0]-=torqueCon[treeStorage[bitMask][1]+10][0];
			torque[1]-=torqueCon[treeStorage[bitMask][1]+10][1];
			torqueCon[treeStorage[bitMask][1]+10][0]=0;
			torqueCon[treeStorage[bitMask][1]+10][1]=1;
			positions[treeStorage[bitMask][1]+10]=0;
			return (treeStorage[bitMask][1])+" "+w;
		}
		recursiveRmBlue(bitMask, copy(positions), weightOnLever,copy(torque),true);
		if(treeStorage[bitMask][0]!=0)//hopefully is one , else lose for certain
		{
			if(treeStorage[bitMask][0]==-1)
			{
				System.out.println("!!!");
			}
			int w=positions[treeStorage[bitMask][1]+10];
			weightOnLever--;
			notInSetOne[treeStorage[bitMask][1]+10]=false;
			inSetOne[treeStorage[bitMask][1]+10]=false;
			torque[0]-=torqueCon[treeStorage[bitMask][1]+10][0];
			torque[1]-=torqueCon[treeStorage[bitMask][1]+10][1];
			torqueCon[treeStorage[bitMask][1]+10][0]=0;
			torqueCon[treeStorage[bitMask][1]+10][1]=1;
			positions[treeStorage[bitMask][1]+10]=0;
			return (treeStorage[bitMask][1])+" "+w;
		}
		return "error removeBlue";
	}
	
	public void recursiveRmBlue(int bm,int[] pos, int wLeft, int[] t, boolean max)
	{
		if(t[0]>0||t[1]<0)
		{
			treeStorage[bm][0]=TIPPED;
			return;
		}
		if(wLeft==1)
		{
			treeStorage[bm][0]=1;
			for(int i=0;i<21;i++)
			{
				if(pos[i]!=0)
				{
					treeStorage[bm][1]=i-10;
					return;
				}
			}
		}
		if(max)
		{
			int maxNum=-1500;
			int move=-50;
			for(int i=0;i<21;i++)
			{
				if(pos[i]!=0&&notInSetOne[i])
				{//blue max remove notInSetOne from left side to right
					int bitMask=bm;
					bitMask^=mask[positionAndMask[i]];
					if(treeStorage[bitMask][0]==0)
					{
						int w=pos[i];
						pos[i]=0;
						t[0]-=torqueCon[i][0];
						t[1]-=torqueCon[i][1];
						recursiveRmBlue(bitMask,pos,wLeft-1,t,!max);
						pos[i]=w;
						t[0]+=torqueCon[i][0];
						t[1]+=torqueCon[i][1];
					}
					if(treeStorage[bitMask][0]==1)
					{
						treeStorage[bm][0]=1;
						treeStorage[bm][1]=i-10;
						return;
					}else if(treeStorage[bitMask][0]==-1)
					{
						maxNum=-1;
						move=i-10;
					}else if(treeStorage[bitMask][0]==TIPPED)
					{
						maxNum=-1;
						if(move<-10)
						{
							move=i-10;
						}
					}
				}
			}
			for(int i=20;i>=0;i--)
			{
				if(pos[i]!=0)
				{//blue max remove anything
					int bitMask=bm;
					bitMask^=mask[positionAndMask[i]];
					if(treeStorage[bitMask][0]==0)
					{
						int w=pos[i];
						pos[i]=0;
						t[0]-=torqueCon[i][0];
						t[1]-=torqueCon[i][1];
						recursiveRmBlue(bitMask,pos,wLeft-1,t,!max);
						pos[i]=w;
						t[0]+=torqueCon[i][0];
						t[1]+=torqueCon[i][1];
					}
					if(treeStorage[bitMask][0]==1)
					{
						treeStorage[bm][0]=1;
						treeStorage[bm][1]=i-10;
						return;
					}else if(treeStorage[bitMask][0]==-1)
					{
						maxNum=-1;
						move=i-10;
					}else if(treeStorage[bitMask][0]==TIPPED)
					{
						maxNum=-1;
						if(move<-10)
						{
							move=i-10;
						}
					}
				
				}
			}
			treeStorage[bm][0]=maxNum;
			treeStorage[bm][1]=move;
			return;
		}else
		{
			int maxNum=5;
			int move=-50;
			for(int i=0;i<21;i++)
			{//blue min remove set1
				if(pos[i]!=0&&inSetOne[i])
				{
					int bitMask=bm;
					bitMask^=mask[positionAndMask[i]];
					if(treeStorage[bitMask][0]==0)
					{
						int w=pos[i];
						pos[i]=0;
						t[0]-=torqueCon[i][0];
						t[1]-=torqueCon[i][1];
						recursiveRmBlue(bitMask,pos,wLeft-1,t,!max);
						pos[i]=w;
						t[0]+=torqueCon[i][0];
						t[1]+=torqueCon[i][1];
					}
					if(treeStorage[bitMask][0]==-1)
					{
						treeStorage[bm][0]=-1;
						treeStorage[bm][1]=i-10;
						return;
					}else if(treeStorage[bitMask][0]==1)
					{
						maxNum=1;
						move=i-10;
					}else if(treeStorage[bitMask][0]==TIPPED)
					{
						maxNum=1;
						if(move<-10)
						{
							move=i-10;
						}
					}
					
				}
			}
			for(int i=20;i>=0;i--)
			{//blue min remove from right to left
				if(pos[i]!=0)
				{
					int bitMask=bm;
					bitMask^=mask[positionAndMask[i]];
					if(treeStorage[bitMask][0]==0)
					{
						int w=pos[i];
						pos[i]=0;
						t[0]-=torqueCon[i][0];
						t[1]-=torqueCon[i][1];
						recursiveRmBlue(bitMask,pos,wLeft-1,t,!max);
						pos[i]=w;
						t[0]+=torqueCon[i][0];
						t[1]+=torqueCon[i][1];
					}
					if(treeStorage[bitMask][0]==-1)
					{
						treeStorage[bm][0]=-1;
						treeStorage[bm][1]=i-10;
						return;
					}else if(treeStorage[bitMask][0]==1)
					{
						maxNum=1;
						move=i-10;
					}else if(treeStorage[bitMask][0]==TIPPED)
					{
						maxNum=1;
						if(move<-10)
						{
							move=i-10;
						}
					}
				}
			}

			treeStorage[bm][0]=maxNum;
			treeStorage[bm][1]=move;
			return;
		}
	}
	
	public String simpleRemoveRed()
	{
System.out.print(" simple rm red ");
		for(int i=-2;i>=-10;i--)
		{
			if(inSetOne[i+10]&&torque[0]-torqueCon[i+10][0]<=0&&
					torque[1]-torqueCon[i+10][1]>=0)
			{
				int w=positions[i+10];
				weightOnLever--;
				inSetOne[i+10]=false;
				torque[0]-=torqueCon[i+10][0];
				torque[1]-=torqueCon[i+10][1];
				torqueCon[i+10][0]=0;
				torqueCon[i+10][1]=1;
				positions[i+10]=0;
				return (i+10)+" "+w;
			}
		}
		for(int i=10;i>=-10;i--)
		{
			if(notInSetOne[i+10]&&torque[0]-torqueCon[i+10][0]<=0&&
					torque[1]-torqueCon[i+10][1]>=0)
			{
				int w=positions[i+10];
				weightOnLever--;
				notInSetOne[i+10]=false;
				torque[0]-=torqueCon[i+10][0];
				torque[1]-=torqueCon[i+10][1];
				torqueCon[i+10][0]=0;
				torqueCon[i+10][1]=1;
				positions[i+10]=0;
				return (i+10)+" "+w;
			}
		}
		return "error simple rm red";
	}
	
	public String simpleRemoveBlue()
	{
System.out.print(" simple rm blue ");
		for(int i=-10;i<=10;i++)
		{
			if(notInSetOne[i+10]&&torque[0]-torqueCon[i+10][0]<=0&&
					torque[1]-torqueCon[i+10][1]>=0)
			{
				int w=positions[i+10];
				weightOnLever--;
				inSetOne[i+10]=false;
				torque[0]-=torqueCon[i+10][0];
				torque[1]-=torqueCon[i+10][1];
				torqueCon[i+10][0]=0;
				torqueCon[i+10][1]=1;
				positions[i+10]=0;
				return (i+10)+" "+w;
			}
		}
		for(int i=10;i>=-10;i--)
		{
			if(inSetOne[i+10]&&torque[0]-torqueCon[i+10][0]<=0&&
					torque[1]-torqueCon[i+10][1]>=0)
			{
System.out.print("  rm set1 in blue  ");
				int w=positions[i+10];
				weightOnLever--;
				inSetOne[i+10]=false;
				notInSetOne[i+10]=false;
				torque[0]-=torqueCon[i+10][0];
				torque[1]-=torqueCon[i+10][1];
				torqueCon[i+10][0]=0;
				torqueCon[i+10][1]=1;
				positions[i+10]=0;
				return (i+10)+" "+w;
			}
		}
		return "error simple rm blue";
	}
	
	//adding phase, play as red, return pos weight
	//place weights on set1 pos that doesn't belong to set1
	//otherwise, still place weight on the left side 
	//so removing set1 weigths would be easier
	public String addRed()
	{
System.out.print(" addRed ");
		for(int i=weightNum-1;i>=0;i--)
		{
			if(redW[i]==NOT_ON_LEVER)
			{
				for(int j=-2;j>=-10;j--)
				{
					int t0=(j-supports[0])*-(i+1);
					int t1=(j-supports[1])*-(i+1);
					if(positions[j+10]==0&&torque[0]+t0<=0&&
							torque[1]+t1>=0&&(!isInSetOne(j,i+1)))
					{
						weightOnLever++;
						notInSetOne[j+10]=true;
						torqueCon[j+10][0]=t0;
						torqueCon[j+10][1]=t1;
						torque[0]+=t0;
						torque[1]+=t1;
						positions[j+10]=i+1;
						redW[i]=j;
						return (j+" "+(i+1));
					}
				}
			}
			
		}
		for(int i=weightNum-1;i>=0;i--)
		{
			if(redW[i]==NOT_ON_LEVER)
			{
				for(int j=-10;j<=10;j++)
				{
					int t0=(j-supports[0])*-(i+1);
					int t1=(j-supports[1])*-(i+1);
					if(positions[j+10]==0&&torque[0]+t0<=0&&
						torque[1]+t1>=0&&(!isInSetOne(j,i+1)))
					{
						weightOnLever++;
						notInSetOne[j+10]=true;
						torqueCon[j+10][0]=t0;
						torqueCon[j+10][1]=t1;
						torque[0]+=t0;
						torque[1]+=t1;
						positions[j+10]=i+1;
						redW[i]=j;
						return (j+" "+(i+1));
					}
				}
			}
			
		}
		for(int i=weightNum-1;i>=0;i--)
		{
			if(redW[i]==NOT_ON_LEVER)
			{
				for(int j=-10;j<=10;j++)
				{
					int t0=(j-supports[0])*-(i+1);
					int t1=(j-supports[1])*-(i+1);
					if(positions[j+10]==0&&torque[0]+t0<=0&&torque[1]+t1>=0)
					{
						weightOnLever++;
						inSetOne[j+10]=true;
						torqueCon[j+10][0]=t0;
						torqueCon[j+10][1]=t1;
						torque[0]+=t0;
						torque[1]+=t1;
						positions[j+10]=i+1;
						redW[i]=j;
						return (j+" "+(i+1));
					}
				}
			}
			
		}
		return "error add red";
	}
	
	//adding phase, play as blue, return pos weight
	//need to keep set1 on the left side, 
	//so if set1 is not possible, put weight on other side 
	public String addBlue()
	{
System.out.print(" add blue ");
		for(int i=weightNum-1;i>=0;i--)
		{
			if(blueW[i]==NOT_ON_LEVER)
			{
				for(int j=set1[i][0];j<=set1[i][1];j++)
				{
					int t0=(j-supports[0])*-(i+1);
					int t1=(j-supports[1])*-(i+1);
					if(positions[j+10]==0&&torque[0]+t0<=0&&torque[1]+t1>=0)
					{
						weightOnLever++;
						inSetOne[j+10]=true;
						torqueCon[j+10][0]=t0;
						torqueCon[j+10][1]=t1;
						torque[0]+=t0;
						torque[1]+=t1;
						positions[j+10]=i+1;
						blueW[i]=j;
						return (j+" "+(i+1));
					}
				}
			}
			
		}
		for(int i=weightNum-1;i>=0;i--)
		{
			if(blueW[i]==NOT_ON_LEVER)
			{
				for(int j=10;j>=-10;j--)
				{
					int t0=(j-supports[0])*-(i+1);
					int t1=(j-supports[1])*-(i+1);
					if(positions[j+10]==0&&torque[0]+t0<=0&&torque[1]+t1>=0)
					{
						weightOnLever++;
						notInSetOne[j+10]=true;
						torqueCon[j+10][0]=t0;
						torqueCon[j+10][1]=t1;
						torque[0]+=t0;
						torque[1]+=t1;
						positions[j+10]=i+1;
						blueW[i]=j;
						return (j+" "+(i+1));
					}
				}
			}
			
		}
		return "error add blue";
	}
	
	public boolean isInSetOne(int p, int w)
	{
		return p>=set1[w-1][0]&&p<=set1[w-1][1];
	}
	

	public int[] copy(int[] array)
	{
		int[] ar=new int[array.length];
		for(int i=0;i<array.length;i++)
		{
			ar[i]=array[i];
		}
		return ar;
	}
	

	
	//return true if the weights placed in pos is safe from tipping
	//pos[position +10]= weight number, index-10= position of weight
	public boolean safeFromTipping(int[] pos)
	{
		int t[]={-9,-3};
		for(int i=0;i<pos.length;i++)
		{
			t[0]+=((i-10)-supports[0])*-pos[i];
			t[1]+=((i-10)-supports[1])*-pos[i];
			//t[0]+=(10+supports[0]-i)*pos[i];
			//t[1]+=(10+supports[1]-i)*pos[i];
		}
		return (t[0]<=0&&t[1]>=0);
	}
}
