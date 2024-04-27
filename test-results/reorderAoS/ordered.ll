; ModuleID = 'ordered.c'
source_filename = "ordered.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

%struct.nodeOneOld = type { [67 x double], double, double, double, double, [6 x i8], i8, i32, i32, float }

@arrayOne = dso_local global [1000000 x %struct.nodeOneOld] zeroinitializer, align 16
@.str = private unnamed_addr constant [16 x i8] c"Validity check\0A\00", align 1
@.str.1 = private unnamed_addr constant [11 x i8] c"\0Aa: ---\0A%d\00", align 1
@.str.2 = private unnamed_addr constant [11 x i8] c"\0Ab: ---\0A%f\00", align 1
@.str.3 = private unnamed_addr constant [11 x i8] c"\0Ac: ---\0A%d\00", align 1
@.str.4 = private unnamed_addr constant [13 x i8] c"\0Ad: ---\0A%lld\00", align 1
@.str.5 = private unnamed_addr constant [11 x i8] c"\0Ae: ---\0A%d\00", align 1
@.str.6 = private unnamed_addr constant [11 x i8] c"\0Ag: ---\0A%f\00", align 1
@.str.7 = private unnamed_addr constant [11 x i8] c"\0Ah: ---\0A%f\00", align 1
@.str.8 = private unnamed_addr constant [11 x i8] c"\0Ai: ---\0A%f\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @populateNode(ptr noundef %array, i32 noundef %size) #0 {
entry:
  %array.addr = alloca ptr, align 8
  %size.addr = alloca i32, align 4
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  %i23 = alloca i32, align 4
  %j42 = alloca i32, align 4
  store ptr %array, ptr %array.addr, align 8
  store i32 %size, ptr %size.addr, align 4
  store i32 0, ptr %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc20, %entry
  %0 = load i32, ptr %i, align 4
  %1 = load i32, ptr %size.addr, align 4
  %cmp = icmp slt i32 %0, %1
  br i1 %cmp, label %for.body, label %for.end22

for.body:                                         ; preds = %for.cond
  %2 = load ptr, ptr %array.addr, align 8
  %3 = load i32, ptr %i, align 4
  %idxprom = sext i32 %3 to i64
  %arrayidx = getelementptr inbounds %struct.nodeOneOld, ptr %2, i64 %idxprom
  %a = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx, i32 0, i32 7
  store i32 1, ptr %a, align 8
  %4 = load ptr, ptr %array.addr, align 8
  %5 = load i32, ptr %i, align 4
  %idxprom1 = sext i32 %5 to i64
  %arrayidx2 = getelementptr inbounds %struct.nodeOneOld, ptr %4, i64 %idxprom1
  %b = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx2, i32 0, i32 1
  store double 1.000000e+01, ptr %b, align 8
  %6 = load ptr, ptr %array.addr, align 8
  %7 = load i32, ptr %i, align 4
  %idxprom3 = sext i32 %7 to i64
  %arrayidx4 = getelementptr inbounds %struct.nodeOneOld, ptr %6, i64 %idxprom3
  %c = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx4, i32 0, i32 8
  store i32 9, ptr %c, align 4
  %8 = load ptr, ptr %array.addr, align 8
  %9 = load i32, ptr %i, align 4
  %idxprom5 = sext i32 %9 to i64
  %arrayidx6 = getelementptr inbounds %struct.nodeOneOld, ptr %8, i64 %idxprom5
  %d = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx6, i32 0, i32 5
  store i48 23, ptr %d, align 8
  %10 = load ptr, ptr %array.addr, align 8
  %11 = load i32, ptr %i, align 4
  %idxprom7 = sext i32 %11 to i64
  %arrayidx8 = getelementptr inbounds %struct.nodeOneOld, ptr %10, i64 %idxprom7
  %e = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx8, i32 0, i32 6
  store i8 97, ptr %e, align 2
  store i32 0, ptr %j, align 4
  br label %for.cond9

for.cond9:                                        ; preds = %for.inc, %for.body
  %12 = load i32, ptr %j, align 4
  %cmp10 = icmp slt i32 %12, 2
  br i1 %cmp10, label %for.body11, label %for.end

for.body11:                                       ; preds = %for.cond9
  %13 = load ptr, ptr %array.addr, align 8
  %14 = load i32, ptr %i, align 4
  %idxprom12 = sext i32 %14 to i64
  %arrayidx13 = getelementptr inbounds %struct.nodeOneOld, ptr %13, i64 %idxprom12
  %f = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx13, i32 0, i32 0
  %15 = load i32, ptr %j, align 4
  %idxprom14 = sext i32 %15 to i64
  %arrayidx15 = getelementptr inbounds [67 x double], ptr %f, i64 0, i64 %idxprom14
  store double 2.300000e+01, ptr %arrayidx15, align 8
  br label %for.inc

for.inc:                                          ; preds = %for.body11
  %16 = load i32, ptr %j, align 4
  %inc = add nsw i32 %16, 1
  store i32 %inc, ptr %j, align 4
  br label %for.cond9, !llvm.loop !4

for.end:                                          ; preds = %for.cond9
  %17 = load ptr, ptr %array.addr, align 8
  %18 = load i32, ptr %i, align 4
  %idxprom16 = sext i32 %18 to i64
  %arrayidx17 = getelementptr inbounds %struct.nodeOneOld, ptr %17, i64 %idxprom16
  %g = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx17, i32 0, i32 2
  store double 2.300000e+01, ptr %g, align 8
  %19 = load ptr, ptr %array.addr, align 8
  %20 = load i32, ptr %i, align 4
  %idxprom18 = sext i32 %20 to i64
  %arrayidx19 = getelementptr inbounds %struct.nodeOneOld, ptr %19, i64 %idxprom18
  %h = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx19, i32 0, i32 3
  store double 0x407C852CA57A786C, ptr %h, align 8
  br label %for.inc20

for.inc20:                                        ; preds = %for.end
  %21 = load i32, ptr %i, align 4
  %inc21 = add nsw i32 %21, 1
  store i32 %inc21, ptr %i, align 4
  br label %for.cond, !llvm.loop !6

for.end22:                                        ; preds = %for.cond
  store i32 0, ptr %i23, align 4
  br label %for.cond24

for.cond24:                                       ; preds = %for.inc60, %for.end22
  %22 = load i32, ptr %i23, align 4
  %23 = load i32, ptr %size.addr, align 4
  %cmp25 = icmp slt i32 %22, %23
  br i1 %cmp25, label %for.body26, label %for.end62

for.body26:                                       ; preds = %for.cond24
  %24 = load i32, ptr %i23, align 4
  %idxprom27 = sext i32 %24 to i64
  %arrayidx28 = getelementptr inbounds [1000000 x %struct.nodeOneOld], ptr @arrayOne, i64 0, i64 %idxprom27
  %a29 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx28, i32 0, i32 7
  store i32 1, ptr %a29, align 16
  %25 = load i32, ptr %i23, align 4
  %idxprom30 = sext i32 %25 to i64
  %arrayidx31 = getelementptr inbounds [1000000 x %struct.nodeOneOld], ptr @arrayOne, i64 0, i64 %idxprom30
  %b32 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx31, i32 0, i32 1
  store double 1.000000e+01, ptr %b32, align 8
  %26 = load i32, ptr %i23, align 4
  %idxprom33 = sext i32 %26 to i64
  %arrayidx34 = getelementptr inbounds [1000000 x %struct.nodeOneOld], ptr @arrayOne, i64 0, i64 %idxprom33
  %c35 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx34, i32 0, i32 8
  store i32 9, ptr %c35, align 4
  %27 = load i32, ptr %i23, align 4
  %idxprom36 = sext i32 %27 to i64
  %arrayidx37 = getelementptr inbounds [1000000 x %struct.nodeOneOld], ptr @arrayOne, i64 0, i64 %idxprom36
  %d38 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx37, i32 0, i32 5
  store i48 23, ptr %d38, align 8
  %28 = load i32, ptr %i23, align 4
  %idxprom39 = sext i32 %28 to i64
  %arrayidx40 = getelementptr inbounds [1000000 x %struct.nodeOneOld], ptr @arrayOne, i64 0, i64 %idxprom39
  %e41 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx40, i32 0, i32 6
  store i8 97, ptr %e41, align 2
  store i32 0, ptr %j42, align 4
  br label %for.cond43

for.cond43:                                       ; preds = %for.inc51, %for.body26
  %29 = load i32, ptr %j42, align 4
  %cmp44 = icmp slt i32 %29, 2
  br i1 %cmp44, label %for.body45, label %for.end53

for.body45:                                       ; preds = %for.cond43
  %30 = load ptr, ptr %array.addr, align 8
  %31 = load i32, ptr %i23, align 4
  %idxprom46 = sext i32 %31 to i64
  %arrayidx47 = getelementptr inbounds %struct.nodeOneOld, ptr %30, i64 %idxprom46
  %f48 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx47, i32 0, i32 0
  %32 = load i32, ptr %j42, align 4
  %idxprom49 = sext i32 %32 to i64
  %arrayidx50 = getelementptr inbounds [67 x double], ptr %f48, i64 0, i64 %idxprom49
  store double 2.300000e+01, ptr %arrayidx50, align 8
  br label %for.inc51

for.inc51:                                        ; preds = %for.body45
  %33 = load i32, ptr %j42, align 4
  %inc52 = add nsw i32 %33, 1
  store i32 %inc52, ptr %j42, align 4
  br label %for.cond43, !llvm.loop !7

for.end53:                                        ; preds = %for.cond43
  %34 = load i32, ptr %i23, align 4
  %idxprom54 = sext i32 %34 to i64
  %arrayidx55 = getelementptr inbounds [1000000 x %struct.nodeOneOld], ptr @arrayOne, i64 0, i64 %idxprom54
  %g56 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx55, i32 0, i32 2
  store double 2.300000e+01, ptr %g56, align 16
  %35 = load i32, ptr %i23, align 4
  %idxprom57 = sext i32 %35 to i64
  %arrayidx58 = getelementptr inbounds [1000000 x %struct.nodeOneOld], ptr @arrayOne, i64 0, i64 %idxprom57
  %h59 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx58, i32 0, i32 3
  store double 0x407C852CA57A786C, ptr %h59, align 8
  br label %for.inc60

for.inc60:                                        ; preds = %for.end53
  %36 = load i32, ptr %i23, align 4
  %inc61 = add nsw i32 %36, 1
  store i32 %inc61, ptr %i23, align 4
  br label %for.cond24, !llvm.loop !8

for.end62:                                        ; preds = %for.cond24
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @calculationsOne(i32 noundef %size) #0 {
entry:
  %size.addr = alloca i32, align 4
  %j = alloca i32, align 4
  %i = alloca i32, align 4
  %j13 = alloca i32, align 4
  store i32 %size, ptr %size.addr, align 4
  store i32 0, ptr %j, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc31, %entry
  %0 = load i32, ptr %j, align 4
  %cmp = icmp slt i32 %0, 100000
  br i1 %cmp, label %for.body, label %for.end33

for.body:                                         ; preds = %for.cond
  store i32 0, ptr %i, align 4
  br label %for.cond1

for.cond1:                                        ; preds = %for.inc28, %for.body
  %1 = load i32, ptr %i, align 4
  %2 = load i32, ptr %size.addr, align 4
  %cmp2 = icmp slt i32 %1, %2
  br i1 %cmp2, label %for.body3, label %for.end30

for.body3:                                        ; preds = %for.cond1
  %3 = load i32, ptr %i, align 4
  %idxprom = sext i32 %3 to i64
  %arrayidx = getelementptr inbounds [1000000 x %struct.nodeOneOld], ptr @arrayOne, i64 0, i64 %idxprom
  %a = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx, i32 0, i32 7
  %4 = load i32, ptr %a, align 16
  %add = add nsw i32 %4, 1
  store i32 %add, ptr %a, align 16
  %5 = load i32, ptr %i, align 4
  %idxprom4 = sext i32 %5 to i64
  %arrayidx5 = getelementptr inbounds [1000000 x %struct.nodeOneOld], ptr @arrayOne, i64 0, i64 %idxprom4
  %b = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx5, i32 0, i32 1
  %6 = load double, ptr %b, align 8
  %add6 = fadd double %6, 1.000000e-03
  store double %add6, ptr %b, align 8
  %7 = load i32, ptr %i, align 4
  %idxprom7 = sext i32 %7 to i64
  %arrayidx8 = getelementptr inbounds [1000000 x %struct.nodeOneOld], ptr @arrayOne, i64 0, i64 %idxprom7
  %c = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx8, i32 0, i32 8
  %8 = load i32, ptr %c, align 4
  %add9 = add nsw i32 %8, 1
  store i32 %add9, ptr %c, align 4
  %9 = load i32, ptr %i, align 4
  %idxprom10 = sext i32 %9 to i64
  %arrayidx11 = getelementptr inbounds [1000000 x %struct.nodeOneOld], ptr @arrayOne, i64 0, i64 %idxprom10
  %d = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx11, i32 0, i32 5
  %bf.load = load i48, ptr %d, align 8
  %bf.cast = sext i48 %bf.load to i64
  %add12 = add nsw i64 %bf.cast, 1
  %10 = trunc i64 %add12 to i48
  store i48 %10, ptr %d, align 8
  %bf.result.cast = sext i48 %10 to i64
  store i32 0, ptr %j13, align 4
  br label %for.cond14

for.cond14:                                       ; preds = %for.inc, %for.body3
  %11 = load i32, ptr %j13, align 4
  %cmp15 = icmp slt i32 %11, 2
  br i1 %cmp15, label %for.body16, label %for.end

for.body16:                                       ; preds = %for.cond14
  %12 = load i32, ptr %i, align 4
  %idxprom17 = sext i32 %12 to i64
  %arrayidx18 = getelementptr inbounds [1000000 x %struct.nodeOneOld], ptr @arrayOne, i64 0, i64 %idxprom17
  %f = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx18, i32 0, i32 0
  %13 = load i32, ptr %j13, align 4
  %idxprom19 = sext i32 %13 to i64
  %arrayidx20 = getelementptr inbounds [67 x double], ptr %f, i64 0, i64 %idxprom19
  %14 = load double, ptr %arrayidx20, align 8
  %add21 = fadd double %14, 1.000000e-03
  store double %add21, ptr %arrayidx20, align 8
  br label %for.inc

for.inc:                                          ; preds = %for.body16
  %15 = load i32, ptr %j13, align 4
  %inc = add nsw i32 %15, 1
  store i32 %inc, ptr %j13, align 4
  br label %for.cond14, !llvm.loop !9

for.end:                                          ; preds = %for.cond14
  %16 = load i32, ptr %i, align 4
  %idxprom22 = sext i32 %16 to i64
  %arrayidx23 = getelementptr inbounds [1000000 x %struct.nodeOneOld], ptr @arrayOne, i64 0, i64 %idxprom22
  %g = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx23, i32 0, i32 2
  %17 = load double, ptr %g, align 16
  %add24 = fadd double %17, 1.000000e-03
  store double %add24, ptr %g, align 16
  %18 = load i32, ptr %i, align 4
  %idxprom25 = sext i32 %18 to i64
  %arrayidx26 = getelementptr inbounds [1000000 x %struct.nodeOneOld], ptr @arrayOne, i64 0, i64 %idxprom25
  %h = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx26, i32 0, i32 3
  %19 = load double, ptr %h, align 8
  %add27 = fadd double %19, 1.000000e-03
  store double %add27, ptr %h, align 8
  br label %for.inc28

for.inc28:                                        ; preds = %for.end
  %20 = load i32, ptr %i, align 4
  %inc29 = add nsw i32 %20, 1
  store i32 %inc29, ptr %i, align 4
  br label %for.cond1, !llvm.loop !10

for.end30:                                        ; preds = %for.cond1
  br label %for.inc31

for.inc31:                                        ; preds = %for.end30
  %21 = load i32, ptr %j, align 4
  %inc32 = add nsw i32 %21, 1
  store i32 %inc32, ptr %j, align 4
  br label %for.cond, !llvm.loop !11

for.end33:                                        ; preds = %for.cond
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @calculationsTwo(i32 noundef %size, ptr noundef %array) #0 {
entry:
  %size.addr = alloca i32, align 4
  %array.addr = alloca ptr, align 8
  %j = alloca i32, align 4
  %i = alloca i32, align 4
  %j27 = alloca i32, align 4
  store i32 %size, ptr %size.addr, align 4
  store ptr %array, ptr %array.addr, align 8
  store i32 0, ptr %j, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc56, %entry
  %0 = load i32, ptr %j, align 4
  %cmp = icmp slt i32 %0, 100000
  br i1 %cmp, label %for.body, label %for.end58

for.body:                                         ; preds = %for.cond
  store i32 0, ptr %i, align 4
  br label %for.cond1

for.cond1:                                        ; preds = %for.inc53, %for.body
  %1 = load i32, ptr %i, align 4
  %2 = load i32, ptr %size.addr, align 4
  %cmp2 = icmp slt i32 %1, %2
  br i1 %cmp2, label %for.body3, label %for.end55

for.body3:                                        ; preds = %for.cond1
  %3 = load i32, ptr %i, align 4
  %idxprom = sext i32 %3 to i64
  %arrayidx = getelementptr inbounds [1000000 x %struct.nodeOneOld], ptr @arrayOne, i64 0, i64 %idxprom
  %a = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx, i32 0, i32 7
  %4 = load i32, ptr %a, align 16
  %5 = load ptr, ptr %array.addr, align 8
  %6 = load i32, ptr %i, align 4
  %idxprom4 = sext i32 %6 to i64
  %arrayidx5 = getelementptr inbounds %struct.nodeOneOld, ptr %5, i64 %idxprom4
  %a6 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx5, i32 0, i32 7
  %7 = load i32, ptr %a6, align 8
  %add = add nsw i32 %7, %4
  store i32 %add, ptr %a6, align 8
  %8 = load i32, ptr %i, align 4
  %idxprom7 = sext i32 %8 to i64
  %arrayidx8 = getelementptr inbounds [1000000 x %struct.nodeOneOld], ptr @arrayOne, i64 0, i64 %idxprom7
  %b = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx8, i32 0, i32 1
  %9 = load double, ptr %b, align 8
  %10 = load ptr, ptr %array.addr, align 8
  %11 = load i32, ptr %i, align 4
  %idxprom9 = sext i32 %11 to i64
  %arrayidx10 = getelementptr inbounds %struct.nodeOneOld, ptr %10, i64 %idxprom9
  %b11 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx10, i32 0, i32 1
  %12 = load double, ptr %b11, align 8
  %add12 = fadd double %12, %9
  store double %add12, ptr %b11, align 8
  %13 = load i32, ptr %i, align 4
  %idxprom13 = sext i32 %13 to i64
  %arrayidx14 = getelementptr inbounds [1000000 x %struct.nodeOneOld], ptr @arrayOne, i64 0, i64 %idxprom13
  %c = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx14, i32 0, i32 8
  %14 = load i32, ptr %c, align 4
  %15 = load ptr, ptr %array.addr, align 8
  %16 = load i32, ptr %i, align 4
  %idxprom15 = sext i32 %16 to i64
  %arrayidx16 = getelementptr inbounds %struct.nodeOneOld, ptr %15, i64 %idxprom15
  %c17 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx16, i32 0, i32 8
  %17 = load i32, ptr %c17, align 4
  %add18 = add nsw i32 %17, %14
  store i32 %add18, ptr %c17, align 4
  %18 = load i32, ptr %i, align 4
  %idxprom19 = sext i32 %18 to i64
  %arrayidx20 = getelementptr inbounds [1000000 x %struct.nodeOneOld], ptr @arrayOne, i64 0, i64 %idxprom19
  %d = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx20, i32 0, i32 5
  %bf.load = load i48, ptr %d, align 8
  %bf.cast = sext i48 %bf.load to i64
  %19 = load ptr, ptr %array.addr, align 8
  %20 = load i32, ptr %i, align 4
  %idxprom21 = sext i32 %20 to i64
  %arrayidx22 = getelementptr inbounds %struct.nodeOneOld, ptr %19, i64 %idxprom21
  %d23 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx22, i32 0, i32 5
  %bf.load24 = load i48, ptr %d23, align 8
  %bf.cast25 = sext i48 %bf.load24 to i64
  %add26 = add nsw i64 %bf.cast25, %bf.cast
  %21 = trunc i64 %add26 to i48
  store i48 %21, ptr %d23, align 8
  %bf.result.cast = sext i48 %21 to i64
  store i32 0, ptr %j27, align 4
  br label %for.cond28

for.cond28:                                       ; preds = %for.inc, %for.body3
  %22 = load i32, ptr %j27, align 4
  %cmp29 = icmp slt i32 %22, 2
  br i1 %cmp29, label %for.body30, label %for.end

for.body30:                                       ; preds = %for.cond28
  %23 = load i32, ptr %i, align 4
  %idxprom31 = sext i32 %23 to i64
  %arrayidx32 = getelementptr inbounds [1000000 x %struct.nodeOneOld], ptr @arrayOne, i64 0, i64 %idxprom31
  %f = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx32, i32 0, i32 0
  %24 = load i32, ptr %j27, align 4
  %idxprom33 = sext i32 %24 to i64
  %arrayidx34 = getelementptr inbounds [67 x double], ptr %f, i64 0, i64 %idxprom33
  %25 = load double, ptr %arrayidx34, align 8
  %26 = load ptr, ptr %array.addr, align 8
  %27 = load i32, ptr %i, align 4
  %idxprom35 = sext i32 %27 to i64
  %arrayidx36 = getelementptr inbounds %struct.nodeOneOld, ptr %26, i64 %idxprom35
  %f37 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx36, i32 0, i32 0
  %28 = load i32, ptr %j27, align 4
  %idxprom38 = sext i32 %28 to i64
  %arrayidx39 = getelementptr inbounds [67 x double], ptr %f37, i64 0, i64 %idxprom38
  %29 = load double, ptr %arrayidx39, align 8
  %add40 = fadd double %29, %25
  store double %add40, ptr %arrayidx39, align 8
  br label %for.inc

for.inc:                                          ; preds = %for.body30
  %30 = load i32, ptr %j27, align 4
  %inc = add nsw i32 %30, 1
  store i32 %inc, ptr %j27, align 4
  br label %for.cond28, !llvm.loop !12

for.end:                                          ; preds = %for.cond28
  %31 = load i32, ptr %i, align 4
  %idxprom41 = sext i32 %31 to i64
  %arrayidx42 = getelementptr inbounds [1000000 x %struct.nodeOneOld], ptr @arrayOne, i64 0, i64 %idxprom41
  %g = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx42, i32 0, i32 2
  %32 = load double, ptr %g, align 16
  %33 = load ptr, ptr %array.addr, align 8
  %34 = load i32, ptr %i, align 4
  %idxprom43 = sext i32 %34 to i64
  %arrayidx44 = getelementptr inbounds %struct.nodeOneOld, ptr %33, i64 %idxprom43
  %g45 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx44, i32 0, i32 2
  %35 = load double, ptr %g45, align 8
  %add46 = fadd double %35, %32
  store double %add46, ptr %g45, align 8
  %36 = load i32, ptr %i, align 4
  %idxprom47 = sext i32 %36 to i64
  %arrayidx48 = getelementptr inbounds [1000000 x %struct.nodeOneOld], ptr @arrayOne, i64 0, i64 %idxprom47
  %h = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx48, i32 0, i32 3
  %37 = load double, ptr %h, align 8
  %38 = load ptr, ptr %array.addr, align 8
  %39 = load i32, ptr %i, align 4
  %idxprom49 = sext i32 %39 to i64
  %arrayidx50 = getelementptr inbounds %struct.nodeOneOld, ptr %38, i64 %idxprom49
  %h51 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx50, i32 0, i32 3
  %40 = load double, ptr %h51, align 8
  %add52 = fadd double %40, %37
  store double %add52, ptr %h51, align 8
  br label %for.inc53

for.inc53:                                        ; preds = %for.end
  %41 = load i32, ptr %i, align 4
  %inc54 = add nsw i32 %41, 1
  store i32 %inc54, ptr %i, align 4
  br label %for.cond1, !llvm.loop !13

for.end55:                                        ; preds = %for.cond1
  br label %for.inc56

for.inc56:                                        ; preds = %for.end55
  %42 = load i32, ptr %j, align 4
  %inc57 = add nsw i32 %42, 1
  store i32 %inc57, ptr %j, align 4
  br label %for.cond, !llvm.loop !14

for.end58:                                        ; preds = %for.cond
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main(i32 noundef %argc, ptr noundef %argv) #0 {
entry:
  %retval = alloca i32, align 4
  %argc.addr = alloca i32, align 4
  %argv.addr = alloca ptr, align 8
  %n = alloca i32, align 4
  %arrayTwoOld = alloca ptr, align 8
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  store i32 0, ptr %retval, align 4
  store i32 %argc, ptr %argc.addr, align 4
  store ptr %argv, ptr %argv.addr, align 8
  %0 = load ptr, ptr %argv.addr, align 8
  %arrayidx = getelementptr inbounds ptr, ptr %0, i64 1
  %1 = load ptr, ptr %arrayidx, align 8
  %call = call i32 @atoi(ptr noundef %1) #5
  store i32 %call, ptr %n, align 4
  %2 = load i32, ptr %n, align 4
  %conv = sext i32 %2 to i64
  %mul = mul i64 %conv, 592
  %call1 = call noalias ptr @malloc(i64 noundef %mul) #6
  store ptr %call1, ptr %arrayTwoOld, align 8
  %3 = load ptr, ptr %arrayTwoOld, align 8
  %4 = load i32, ptr %n, align 4
  call void @populateNode(ptr noundef %3, i32 noundef %4)
  %5 = load i32, ptr %n, align 4
  call void @calculationsOne(i32 noundef %5)
  %6 = load i32, ptr %n, align 4
  %7 = load ptr, ptr %arrayTwoOld, align 8
  call void @calculationsTwo(i32 noundef %6, ptr noundef %7)
  %call2 = call i32 @rand() #7
  %8 = load i32, ptr %n, align 4
  %rem = srem i32 %call2, %8
  store i32 %rem, ptr %i, align 4
  %call3 = call i32 @rand() #7
  %rem4 = srem i32 %call3, 67
  store i32 %rem4, ptr %j, align 4
  %call5 = call i32 (ptr, ...) @printf(ptr noundef @.str)
  %9 = load ptr, ptr %arrayTwoOld, align 8
  %10 = load i32, ptr %i, align 4
  %idxprom = sext i32 %10 to i64
  %arrayidx6 = getelementptr inbounds %struct.nodeOneOld, ptr %9, i64 %idxprom
  %a = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx6, i32 0, i32 7
  %11 = load i32, ptr %a, align 8
  %call7 = call i32 (ptr, ...) @printf(ptr noundef @.str.1, i32 noundef %11)
  %12 = load ptr, ptr %arrayTwoOld, align 8
  %13 = load i32, ptr %i, align 4
  %idxprom8 = sext i32 %13 to i64
  %arrayidx9 = getelementptr inbounds %struct.nodeOneOld, ptr %12, i64 %idxprom8
  %b = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx9, i32 0, i32 1
  %14 = load double, ptr %b, align 8
  %call10 = call i32 (ptr, ...) @printf(ptr noundef @.str.2, double noundef %14)
  %15 = load ptr, ptr %arrayTwoOld, align 8
  %16 = load i32, ptr %i, align 4
  %idxprom11 = sext i32 %16 to i64
  %arrayidx12 = getelementptr inbounds %struct.nodeOneOld, ptr %15, i64 %idxprom11
  %c = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx12, i32 0, i32 8
  %17 = load i32, ptr %c, align 4
  %call13 = call i32 (ptr, ...) @printf(ptr noundef @.str.3, i32 noundef %17)
  %18 = load ptr, ptr %arrayTwoOld, align 8
  %19 = load i32, ptr %i, align 4
  %idxprom14 = sext i32 %19 to i64
  %arrayidx15 = getelementptr inbounds %struct.nodeOneOld, ptr %18, i64 %idxprom14
  %d = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx15, i32 0, i32 5
  %bf.load = load i48, ptr %d, align 8
  %bf.cast = sext i48 %bf.load to i64
  %call16 = call i32 (ptr, ...) @printf(ptr noundef @.str.4, i64 noundef %bf.cast)
  %20 = load ptr, ptr %arrayTwoOld, align 8
  %21 = load i32, ptr %i, align 4
  %idxprom17 = sext i32 %21 to i64
  %arrayidx18 = getelementptr inbounds %struct.nodeOneOld, ptr %20, i64 %idxprom17
  %e = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx18, i32 0, i32 6
  %22 = load i8, ptr %e, align 2
  %conv19 = sext i8 %22 to i32
  %call20 = call i32 (ptr, ...) @printf(ptr noundef @.str.5, i32 noundef %conv19)
  %23 = load ptr, ptr %arrayTwoOld, align 8
  %24 = load i32, ptr %i, align 4
  %idxprom21 = sext i32 %24 to i64
  %arrayidx22 = getelementptr inbounds %struct.nodeOneOld, ptr %23, i64 %idxprom21
  %g = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx22, i32 0, i32 2
  %25 = load double, ptr %g, align 8
  %call23 = call i32 (ptr, ...) @printf(ptr noundef @.str.6, double noundef %25)
  %26 = load ptr, ptr %arrayTwoOld, align 8
  %27 = load i32, ptr %i, align 4
  %idxprom24 = sext i32 %27 to i64
  %arrayidx25 = getelementptr inbounds %struct.nodeOneOld, ptr %26, i64 %idxprom24
  %h = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx25, i32 0, i32 3
  %28 = load double, ptr %h, align 8
  %call26 = call i32 (ptr, ...) @printf(ptr noundef @.str.7, double noundef %28)
  %29 = load ptr, ptr %arrayTwoOld, align 8
  %30 = load i32, ptr %i, align 4
  %idxprom27 = sext i32 %30 to i64
  %arrayidx28 = getelementptr inbounds %struct.nodeOneOld, ptr %29, i64 %idxprom27
  %i29 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx28, i32 0, i32 4
  %31 = load double, ptr %i29, align 8
  %call30 = call i32 (ptr, ...) @printf(ptr noundef @.str.8, double noundef %31)
  ret i32 0
}

; Function Attrs: nounwind willreturn memory(read)
declare dso_local i32 @atoi(ptr noundef) #1

; Function Attrs: nounwind allocsize(0)
declare dso_local noalias ptr @malloc(i64 noundef) #2

; Function Attrs: nounwind
declare dso_local i32 @rand() #3

declare dso_local i32 @printf(ptr noundef, ...) #4

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nounwind willreturn memory(read) "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { nounwind allocsize(0) "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { nounwind "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #4 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #5 = { nounwind willreturn memory(read) }
attributes #6 = { nounwind allocsize(0) }
attributes #7 = { nounwind }

!llvm.module.flags = !{!0, !1, !2}
!llvm.ident = !{!3}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"uwtable", i32 2}
!2 = !{i32 7, !"frame-pointer", i32 2}
!3 = !{!"clang version 16.0.6 (Red Hat 16.0.6-2.module+el8.9.0+1651+e10a8f6d)"}
!4 = distinct !{!4, !5}
!5 = !{!"llvm.loop.mustprogress"}
!6 = distinct !{!6, !5}
!7 = distinct !{!7, !5}
!8 = distinct !{!8, !5}
!9 = distinct !{!9, !5}
!10 = distinct !{!10, !5}
!11 = distinct !{!11, !5}
!12 = distinct !{!12, !5}
!13 = distinct !{!13, !5}
!14 = distinct !{!14, !5}
